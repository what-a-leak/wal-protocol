#include "lora.h"
#include "pin.h"
#include "rf95.h"
// FOR DEBUG PURPOSES ONLY
#include "logger.h"

#include <driver/uart.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>

#define UART_NUM UART_NUM_1
#define BAUD_RATE 57600
#define RH_RF95_FIFO_SIZE 255
#define RH_RF95_HEADER_LEN 4
#define RH_RF95_MAX_PAYLOAD_LEN RH_RF95_FIFO_SIZE
#define RH_RF95_MAX_MESSAGE_LEN RH_RF95_MAX_PAYLOAD_LEN-RH_RF95_HEADER_LEN

#define RH_BROADCAST_ADDRESS 0xff

static uint16_t _len_preamble = 8;
static lora_mode_t _mode = INIT_MODE;
static int _rx_buf_valid = 0;
static uint32_t _rx_bad = 0;
static uint32_t _tx_good = 0;
static uint8_t _buf[RH_RF95_MAX_PAYLOAD_LEN];
static uint8_t _buf_len = 0;

// Addresses, ID, Flags...
static uint8_t _this_address = RH_BROADCAST_ADDRESS;
static uint8_t _header_to = RH_BROADCAST_ADDRESS;
static uint8_t _header_from = RH_BROADCAST_ADDRESS;
static uint8_t _header_id = 0;
static uint8_t _header_flags = 0;

/* Read/Write procedures for Groove LoRa */

static inline void lora_write(uint8_t reg, uint8_t val)
{
    const char data[4] = {'W', (uint8_t)(reg | RH_WRITE_MASK), 1, val};
    uart_write_bytes(UART_NUM, data, 4);
}

static inline void lora_burst_write(uint8_t reg, uint8_t* src, uint8_t len)
{
    const char data[3] = {'W', (uint8_t)(reg | RH_WRITE_MASK), len};
    uart_write_bytes(UART_NUM, data, 3);
    uart_write_bytes(UART_NUM, src, len);
}

static inline int16_t lora_read(uint8_t reg)
{
    const char data[3] = {'R', (uint8_t)(reg & ~RH_WRITE_MASK), 1};
    uint8_t buff = {0};
    int len = 0;

    uart_write_bytes(UART_NUM, data, 3);
    if ((len = uart_read_bytes(UART_NUM, &buff, 1, pdMS_TO_TICKS(1000))) > 0)
        return buff;
    else
        return -1;
}

static inline int lora_burst_read(uint8_t reg, uint8_t* dest, uint8_t len)
{
    const char data[3] = {'R', (uint8_t)(reg & ~RH_WRITE_MASK), 1};
    int len_received = 0;

    uart_write_bytes(UART_NUM, data, 3);
    if ((len_received = uart_read_bytes(UART_NUM, &dest, len, pdMS_TO_TICKS(1000))) > 0)
        return len_received;
    else
        return -1;
}

/* Internal functions */

static void lora_setmode_idle()
{
    if (_mode != IDLE_MODE) {
        lora_write(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_STDBY);
        _mode = IDLE_MODE;
    }
}

static void lora_setmode_sleep()
{
    if (_mode != SLEEP_MODE) {
        lora_write(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_SLEEP);
        _mode = SLEEP_MODE;
    }
}

static void lora_setmode_rx()
{
    if (_mode != RX_MODE) {
        lora_write(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_RXCONTINUOUS);
        lora_write(RH_RF95_REG_40_DIO_MAPPING1, 0x00);
        _mode = RX_MODE;
    }
}

static void lora_setmode_tx()
{
    if (_mode != TX_MODE) {
        lora_write(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_TX);
        lora_write(RH_RF95_REG_40_DIO_MAPPING1, 0x40);
        _mode = TX_MODE;
    }
}

void lora_validate_rxbuf() {
    if (_buf_len < 4) {
        return;    // Too short to be a real message
    }

    if (_buf[0] == _this_address || _buf[0] == RH_BROADCAST_ADDRESS)
        _rx_buf_valid = 1;
}

static void lora_handle_interrupt() {
    // Read the interrupt register
    uint8_t irq_flags = lora_read(RH_RF95_REG_12_IRQ_FLAGS);

    if (_mode == RX_MODE && irq_flags & (RH_RF95_RX_TIMEOUT | RH_RF95_PAYLOAD_CRC_ERROR)) {
        _rx_bad++;
    } else if (_mode == RX_MODE && irq_flags & RH_RF95_RX_DONE) {
        // Have received a packet
        uint8_t len = lora_read(RH_RF95_REG_13_RX_NB_BYTES);
        // Reset the fifo this->read ptr to the beginning of the packet
        lora_write(RH_RF95_REG_0D_FIFO_ADDR_PTR, lora_read(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR));
        lora_burst_read(RH_RF95_REG_00_FIFO, _buf, len);
        _buf_len = len;
        lora_write(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags

        // We have received a message.
        lora_validate_rxbuf();
        if (_rx_buf_valid) {
            lora_setmode_idle();    // Got one
        }
    } else if (_mode == TX_MODE && irq_flags & RH_RF95_TX_DONE) {
        _tx_good++;
        lora_setmode_idle();
    }

    lora_write(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
}

/* External functions */

esp_err_t lora_init()
{
    esp_err_t err = ESP_OK;
    // Configure UART parameters
    const uart_config_t uart_config = {
        .baud_rate = 57600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
        .source_clk = UART_SCLK_PLL_F80M};

    // Install UART driver and set UART pins
    if ((err = uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0)) != ESP_OK)
        return err;
    if ((err = uart_param_config(UART_NUM, &uart_config)) != ESP_OK)
        return err;
    err = uart_set_pin(UART_NUM, LORA_TX, LORA_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    return err;
}

esp_err_t lora_test()
{
    int16_t receive = lora_read(RH_RF95_REG_42_VERSION);
    if (receive > 0)
        return ESP_OK;
    else
        return ESP_FAIL;
}

esp_err_t lora_config()
{
    // Setting to Mode Sleep -> Only then we can set to Long Range (Lora Mode).
    lora_write(RH_RF95_REG_01_OP_MODE, RH_RF95_LONG_RANGE_MODE | RH_RF95_MODE_SLEEP);
    // Using the full FIFO 256 bytes.
    lora_write(RH_RF95_REG_0E_FIFO_TX_BASE_ADDR, 0);
    lora_write(RH_RF95_REG_0F_FIFO_RX_BASE_ADDR, 0);
    // Setting preamble length
    lora_write(RH_RF95_REG_20_PREAMBLE_MSB, _len_preamble >> 8);
    lora_write(RH_RF95_REG_21_PREAMBLE_LSB, _len_preamble & 0xff);
    // Verifying that the registers have been correctly modified.
    if (
        (lora_read(RH_RF95_REG_01_OP_MODE) == (RH_RF95_MODE_SLEEP | RH_RF95_LONG_RANGE_MODE))
        && (lora_read(RH_RF95_REG_0E_FIFO_TX_BASE_ADDR) == 0)
        && (lora_read(RH_RF95_REG_0F_FIFO_RX_BASE_ADDR) == 0)
        && (lora_read(RH_RF95_REG_20_PREAMBLE_MSB) == (_len_preamble >> 8))
        && (lora_read(RH_RF95_REG_21_PREAMBLE_LSB) == (_len_preamble & 0xff))
        )
        return ESP_OK;
    else
        return ESP_FAIL;
}

esp_err_t lora_setfrequency(uint16_t frq_mhz)
{
    // 1. converting MHz to Hz
    // 2. f/f_RES = fRF
    uint32_t frf = (frq_mhz * 1000000.0) / F_RES;
    lora_write(RH_RF95_REG_06_FRF_MSB, (frf >> 16) & 0xff);
    lora_write(RH_RF95_REG_07_FRF_MID, (frf >> 8) & 0xff);
    lora_write(RH_RF95_REG_08_FRF_LSB, frf & 0xff);

    uint32_t res =
            ((uint32_t)(lora_read(RH_RF95_REG_06_FRF_MSB) << 16)) 
        +   ((uint16_t)(lora_read(RH_RF95_REG_07_FRF_MID) << 8)) 
        +   lora_read(RH_RF95_REG_08_FRF_LSB);
    if(res == frf)
        return ESP_OK;
    else
        return ESP_FAIL;
}

int lora_available()
{
    uint8_t buff = 'N';
    int len = uart_read_bytes(UART_NUM, &buff, 1, pdMS_TO_TICKS(1000));
    screen_log("[int]: %d", len);
    if(buff == 'I')
        lora_handle_interrupt();

    if (_mode == TX_MODE) {
        return 0;
    }
    lora_setmode_rx();

    return _rx_buf_valid; // Will be set by the interrupt handler when a good message is received
}

esp_err_t lora_send(uint8_t* data, uint8_t len) {
    if (len > RH_RF95_MAX_MESSAGE_LEN) {
        return ESP_FAIL;
    }
    lora_setmode_idle();

    // Position at the beginning of the FIFO
    lora_write(RH_RF95_REG_0D_FIFO_ADDR_PTR, 0);

    // The headers
    lora_write(RH_RF95_REG_00_FIFO, _header_to);
    lora_write(RH_RF95_REG_00_FIFO, _header_from);
    lora_write(RH_RF95_REG_00_FIFO, _header_id);
    lora_write(RH_RF95_REG_00_FIFO, _header_flags);

    // The message data
    lora_burst_write(RH_RF95_REG_00_FIFO, data, len);
    lora_write(RH_RF95_REG_22_PAYLOAD_LENGTH, len + RH_RF95_HEADER_LEN);

    lora_setmode_tx();
    
    // Debug
    uint8_t irq_flags = 0;
    do {
        irq_flags = lora_read(RH_RF95_REG_12_IRQ_FLAGS);
        vTaskDelay(pdMS_TO_TICKS(10)); // Small delay to prevent tight looping
    } while (!irq_flags); // Wait for TxDone
    screen_log("Payload: 0x%x", irq_flags);
    
    lora_write(RH_RF95_REG_12_IRQ_FLAGS, 0xFF); // Clear all flags
    return ESP_OK;
}

esp_err_t lora_recv(uint8_t* buf, uint8_t* len)
{
    if (!lora_available()) {
        return ESP_FAIL;
    }
    if (buf && len) {
        // Skip the 4 headers that are at the beginning of the rxBuf
        if (*len > _buf_len - RH_RF95_HEADER_LEN) {
            *len = _buf_len - RH_RF95_HEADER_LEN;
        }
        memcpy(buf, _buf + RH_RF95_HEADER_LEN, *len);
    }
    _rx_buf_valid = 0;
    _buf_len = 0;
    return ESP_OK;
}
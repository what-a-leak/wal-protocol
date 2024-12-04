#include "lora.h"
#include "pin.h"
#include "rf95.h"
// FOR DEBUG PURPOSES ONLY
#include "logger.h"

#include <driver/uart.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define UART_NUM UART_NUM_1
#define BAUD_RATE 57600

static uint16_t _len_preamble = 8;

static inline void lora_write(uint8_t reg, uint8_t val)
{
    const char data[4] = {'W', (uint8_t)(reg | RH_WRITE_MASK), 1, val};
    uart_write_bytes(UART_NUM, data, 4);
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
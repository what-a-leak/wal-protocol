#include "sx1278_lora.h"
#include "old_sx127x.h"
#include "pin.h"

#include <driver/spi_master.h>

#define DATA_SIZE   2

static spi_device_handle_t spi;
static uint8_t rx_data[DATA_SIZE] = {0};
static uint8_t tx_data[DATA_SIZE] = {0};

static inline uint16_t spi_send(uint8_t reg, uint8_t data)
{
    tx_data[0] = reg & 0x7F;
    tx_data[1] = data;
    spi_transaction_t transaction = {
        .length = DATA_SIZE*8,
        .tx_buffer = tx_data,
        .rx_buffer = rx_data,
    };

    if(spi_device_transmit(spi, &transaction) != ESP_OK)
        return 0;
    else
        return (((uint16_t)rx_data[0] << 8) & 0xff00) + rx_data[1];
}

esp_err_t lora_init(void) {
    esp_err_t ret;
    const spi_bus_config_t buscfg = {
        .miso_io_num = SX1278_PIN_NUM_MISO,
        .mosi_io_num = SX1278_PIN_NUM_MOSI,
        .sclk_io_num = SX1278_PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32
    };
    const spi_device_interface_config_t devcfg = {
        .clock_speed_hz = SPI_MASTER_FREQ_10M, 
        .mode = 0,                        
        .spics_io_num = SX1278_PIN_NUM_CS,
        .queue_size = 1,                  
    };

    ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK)
        return ret;
    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &spi);
    return ret;
}

esp_err_t lora_version(uint8_t* major, uint8_t* minor)
{
    const int16_t res = spi_send(SX1278_REG_VERSION,0x00);
    if(res < 0)
        return ESP_FAIL;
    *minor = res & 0b1111;
    *major = (res & 0b11110000) >> 4;
    return ESP_OK;
}

void dummy_send() {
    spi_send(0x01, 0b10000000); // REG_OP_MODE: LoRa mode, sleep mode

    // Configure frequency (433 MHz)
    spi_send(0x06, 0x6C); // REG_FRF_MSB: 433 MHz frequency
    spi_send(0x07, 0x80); // REG_FRF_MID
    spi_send(0x08, 0x00); // REG_FRF_LSB

    // Configure bandwidth, coding rate, and explicit header mode
    spi_send(0x1D, RADIOLIB_SX1278_BW_125_00_KHZ | RADIOLIB_SX1278_CR_4_5 | RADIOLIB_SX1278_HEADER_EXPL_MODE);
    spi_send(0x1E, (7 << 4) | RADIOLIB_SX1278_RX_CRC_MODE_ON); // REG_MODEM_CONFIG_2: SF7, CRC on
    spi_send(0x09, 0x80 | RADIOLIB_SX1278_MAX_POWER); // REG_PA_CONFIG: PA_BOOST, max power
    spi_send(0x22, 5); // REG_PAYLOAD_LENGTH: 5 bytes
    spi_send(0x0D, 0x00); // REG_FIFO_ADDR_PTR: Reset FIFO pointer

    // Write "Hello" to the FIFO
    const char payload[] = "Hello";
    for (int i = 0; i < 5; i++) {
        spi_send(0x00, payload[i]); // Write each character to the FIFO (REG_FIFO)
    }
    spi_send(0x01, 0b10000011); // REG_OP_MODE: LoRa mode, transmit mode

    int16_t recv = 0;
    int max_attempts = 1000; // Adjust this value based on timing needs
    int attempts = 0;

    while (!(recv = spi_send(0x12, 0x00) & 0x08)) {
        if (recv != 0) {
            printf("Status: 0x%04X...\n", recv);
        }

        attempts++;
        if (attempts > max_attempts) {
            printf("Timeout waiting for TX_DONE flag.\n");
            return; // Exit the function or handle the error
        }
    }
    printf("Data sent.\n");

    // Clear the TX_DONE flag
    spi_send(0x12, 0x08); // REG_IRQ_FLAGS: Clear TX_DONE

    // Set the device back to standby mode
    spi_send(0x01, 0b10000001); // REG_OP_MODE: LoRa mode, standby mode
}


esp_err_t lora_clean(void)
{
    esp_err_t ret = spi_bus_remove_device(spi);
    if (ret != ESP_OK)
        return ret;
    ret = spi_bus_free(SPI2_HOST);
    return ret;
}

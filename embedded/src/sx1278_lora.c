#include "sx1278_lora.h"
#include "pin.h"

#include <stdio.h>
#include <driver/spi_master.h>

static spi_device_handle_t spi;
static uint8_t rx_data[2];

esp_err_t spi_send(uint8_t* tx_data, size_t len)
{
    spi_transaction_t transaction = {
        .length = len*8,
        .tx_buffer = tx_data,
        .rx_buffer = rx_data,
    };

    printf("Send: 0x%02X 0x%02X\n", tx_data[0], tx_data[1]);
    return spi_device_transmit(spi, &transaction);
}

uint8_t* spi_recv(void)
{
    return rx_data;
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

esp_err_t lora_clean(void)
{
    esp_err_t ret = spi_bus_remove_device(spi);
    if (ret != ESP_OK)
        return ret;
    ret = spi_bus_free(SPI2_HOST);
    return ret;
}

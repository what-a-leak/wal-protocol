#include "lora.h"
#include "pin.h"
#include "rf95.h"
// FOR DEBUG PURPOSES ONLY
#include "logger.h"

#include <driver/uart.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define UART_NUM    UART_NUM_1
#define BAUD_RATE   57600

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
        .source_clk = UART_SCLK_PLL_F80M
    };

    // Install UART driver and set UART pins
    if((err = uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0)) != ESP_OK)
        return err;
    if((err = uart_param_config(UART_NUM, &uart_config)) != ESP_OK)
        return err;
    err = uart_set_pin(UART_NUM, LORA_TX, LORA_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    return err;
}

esp_err_t lora_test()
{
    const char data[3] = {'R', (uint8_t)(RH_RF95_REG_42_VERSION & ~RH_WRITE_MASK), 1};
    uart_write_bytes(UART_NUM, data, 3);
    uint8_t receive = 0;
    int len = uart_read_bytes(UART_NUM, &receive, 1, pdMS_TO_TICKS(1000));
    if (len > 0) {
        screen_log("[LoRa] Ver.%x", receive);
        return ESP_OK;
    }
    else {
        screen_log("[LoRa] NO UART");
        return ESP_FAIL;
    }
}
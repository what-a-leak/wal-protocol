#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "sx1278_lora.h"

static esp_err_t err;

void app_main(void)
{
    printf("Lora: Initializing...\n");
    if((err = lora_init()) != ESP_OK)
    {
        printf("Lora: Init failed. (%s)\n", esp_err_to_name(err));
        return;
    }

    uint8_t data_send[2] = {0x42, 0x00};
    uint8_t* data_recv = NULL;
    while (1) {
        if((err = spi_send(data_send, 2)) != ESP_OK)
            printf("Lora: Could not send data.\n");
        else
        {
            data_recv = spi_recv();
            printf("Recv: 0x%02X 0x%02X\n", data_recv[0], data_recv[1]);
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

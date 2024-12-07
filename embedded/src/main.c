#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "sx1278_lora.h"

static esp_err_t err;
void app_main(void)
{
    vTaskDelay(2000/portTICK_PERIOD_MS);
    printf("Lora: Initializing...\n");
    if((err = lora_init()) != ESP_OK)
    {
        printf("Lora: Init failed. (%s)\n", esp_err_to_name(err));
        return;
    }
    
    uint8_t major, minor;
    lora_version(&major,&minor);
    printf("SX1278 Version %d.%d\n", major, minor);

    while (1) {
        dummy_send();
        vTaskDelay(3000/portTICK_PERIOD_MS);
    }
    lora_clean();
}

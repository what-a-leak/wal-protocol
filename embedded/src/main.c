#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "pin.h"
#include "sx1278_lora.h"
#include "screen.h"
#include "logger.h"
#include "button.h"

static esp_err_t err;
static uint32_t count=0;

void setup()
{
    uint8_t major=0, minor=0;

    /* Screen init */
    printf("Screen: Initializing...\n");
    screen_init(SCREEN_SCL,SCREEN_SDA,200000);

    /* LoRa init */
    screen_log("LoRa: Init...");
    if((err = lora_init()) != ESP_OK)
    {
        screen_log("LoRa: Failed.");
        return;
    }
    lora_version(&major,&minor);
    screen_log("LoRa: Ver. %d.%d\n", major, minor);

    /* Button init */
    if((err =button_init(BUTTON)) != ESP_OK)
        screen_log("BTN: Failed.");
    else
        screen_log("BTN: OK.");
}

void loop(void)
{
    if(button_getlevel())
    {
        screen_log("[%d] btn press!", count);
        count++;
    }
    vTaskDelay(100/portTICK_PERIOD_MS);
}

void app_main(void)
{
    setup();
    while (1)
        loop();
    lora_clean();
}

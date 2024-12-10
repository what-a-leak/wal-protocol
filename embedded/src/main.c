#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_task_wdt.h>

#include "pin.h"
#include "sx1278_lora.h"
#include "screen.h"
#include "logger.h"
#include "button.h"

static esp_err_t err;
static uint32_t count=0;
static ButtonState btn = NONE;
#define TIME_MS (xTaskGetTickCount() * 1000 / configTICK_RATE_HZ)

void button_task(void* arg)
{
    int pin_level, pressed=0;
    uint32_t time=0;
    while (1)
    {
        if (xQueueReceive(button_getqueue(), &pin_level, portMAX_DELAY))
        {
            if(pin_level) {
                time = TIME_MS;
                pressed = 1;
            }
            if(!pin_level && pressed) {
                const uint32_t delta = (TIME_MS - time);
                if(delta < 400)
                    btn = PUSH;
                else
                    btn = LONG_PUSH;
                pressed = 0;
            }
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

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
    if((err = button_init(BUTTON)) != ESP_OK)
        screen_log("BTN: Failed.");
    else
        screen_log("BTN: OK.");
    xTaskCreate(button_task, "button_task", 2048, NULL, 1, NULL);
}

void loop(void)
{
    if(btn != NONE) {
        screen_log("[%d]BTN: %d", count, (int)btn);
        count++;
        // Reset
        btn = NONE;
    }
}

void app_main(void)
{
    setup();
    esp_task_wdt_add(NULL);
    while (1) {
        loop();
        esp_task_wdt_reset();
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

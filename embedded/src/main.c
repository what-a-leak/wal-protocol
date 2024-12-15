#define WAL_SEND        0
#define WAL_RECEIVE     1
#define WAL_BUTTON      0

#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_task_wdt.h>

#include "pin.h"
#include "screen.h"
#include "logger.h"
#include "button.h"
#include "test.h"

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

                /* Added functions */
                test_change_mode(btn);
                if(btn == PUSH)
                    test_send_loop(PUSH);
            }
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    esp_err_t err = ESP_OK;
    // Screen init
    printf("Screen: Initializing...\n");
    screen_init(SCREEN_SCL,SCREEN_SDA,200000);
    screen_log("Screen: Init.");

#if WAL_BUTTON
    if((err = button_init(BUTTON)) != ESP_OK)
        screen_log("BTN: Failed.");
    else
        screen_log("BTN: OK.");
    xTaskCreate(button_task, "button_task", 2048, NULL, 1, NULL);
#endif
#if WAL_RECEIVE
    test_recv_setup();
#elif WAL_SEND
    test_send_setup();
#endif
}

void loop(void)
{
#if WAL_RECEIVE
  test_recv_loop();
#elif WAL_SEND
  test_send_loop(btn);
#endif
}

void app_main(void)
{
    setup();
    esp_task_wdt_add(NULL);
    while (1) {
        loop();

        esp_task_wdt_reset();
        vTaskDelay(10/portTICK_PERIOD_MS);
        /* Resetting the button state after everything is called. */
        btn = NONE;
    }
}

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include "sx1278_lora.h"

void app_main(void)
{
    while (1) {
        check_sx1278();
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

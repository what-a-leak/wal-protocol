#include <stdio.h>
#include "pin.h"

// UART stuff
#include "serial.h"
#include "lora.h"

// I2C stuff
#include "screen.h"
#include "logger.h"

#define SEND 0

esp_err_t err = ESP_OK;

void app_main(void)
{
    /* init m8 */
    serial_init(9600, UART_NUM_0);
    screen_init(SCREEN_SCL, SCREEN_SDA, 500000);
    lora_init();

    /* config screen */
    screen_log("Init Screen.");
    
    /* stupid test */
    #if SEND
    stupid_send();
    #else
    stupid_recv();
    #endif

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

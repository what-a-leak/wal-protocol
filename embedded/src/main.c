#include <stdio.h>
#include "pin.h"

// UART stuff
#include "serial.h"
#include "lora.h"

// I2C stuff
#include "screen.h"
#include "logger.h"

esp_err_t err = ESP_OK;

void app_main(void)
{
    /* init m8 */
    serial_init(9600, UART_NUM_0);
    screen_init(SCREEN_SCL, SCREEN_SDA, 500000);
    lora_init();

    /* config screen */
    screen_log("Init Screen.");
    
    /* config LoRa */
    if((err = lora_config()) == ESP_OK)
        screen_log("[LoRa] CONF OK");
    else {
        screen_log("[LoRa] FAILED");
        return;
    }
    if((err = lora_setfrequency(434)) == ESP_OK)
        screen_log("[LoRa] FREQ OK");
    else {
        screen_log("[LoRa] FAILED");
        return;
    }

    /* loop */
    while (1)
    {

    }
}

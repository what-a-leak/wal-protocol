#include <stdio.h>
#include "pin.h"

// UART stuff
#include "serial.h"
#include "lora.h"

// I2C stuff
#include "screen.h"
#include "logger.h"

#define CLIENT  0

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

    const unsigned char send[6] = {'h','e','l','l','o'};
    unsigned char recv[7] = {0};
    uint8_t recv_len = 0;
    /* loop */
    #if CLIENT
    screen_log("Sending...");
    while (1)
    {
        err = lora_send(send, 6);
        if(err == ESP_OK)
            screen_log("Send OK");
        else
            screen_log("Send FAIL");

        // Waiting
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    #else
    screen_log("Receiving...");
    while(1)
    {
        err = lora_recv(recv, &recv_len);
        /*
        if(err == ESP_OK)
            screen_log("Recv OK");
        else
            screen_log("Recv FAIL");
        */

        // Waiting
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    #endif
}

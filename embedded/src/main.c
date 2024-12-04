#include <stdio.h>
#include "pin.h"

// UART stuff
#include "serial.h"
#include "lora.h"

// I2C stuff
#include "screen.h"
#include "logger.h"


void app_main(void)
{
    /* init m8 */
    serial_init(9600, UART_NUM_0);
    screen_init(SCREEN_SCL, SCREEN_SDA, 500000);
    int lora_status = lora_init();

    /* loop */
    screen_log("Init Screen.");
    lora_test();
    while (1)
    {

    }
}

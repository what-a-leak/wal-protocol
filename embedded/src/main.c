#include <stdio.h>
#include "pin.h"
#include "serial.h"
#include "screen.h"
#include "logger.h"

void app_main(void)
{
    /* init m8 */
    serial_init(9600, UART_NUM_0);
    screen_init(SCREEN_SCL, SCREEN_SDA, 500000);

    /* loop */
    screen_log("Init Screen.");
    while (1)
    {

    }
}

#include <stdio.h>
#include "serial.h"
#include "screen.h"

void app_main(void)
{
    /* init m8 */
    serial_init(9600, UART_NUM_0);
    screen_init(GPIO_NUM_6, GPIO_NUM_7, 10000);

    /* loop */
    const char *message = "Hello, World!\n";
    screen_draw();
    while (1)
    {
        uart_write_bytes(UART_NUM_0, message, 15);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
    }
}

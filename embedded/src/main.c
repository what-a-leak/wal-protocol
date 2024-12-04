#include <stdio.h>
#include "serial.h"
#include "screen.h"

void app_main(void)
{
    /* init m8 */
    serial_init(9600, UART_NUM_0);
    screen_init(GPIO_NUM_6, GPIO_NUM_7, 10000);

    /* loop */
    int i= 0;
    while (1)
    {
        screen_draw(i, "Test %d", i);
        //serial_printf("Test %d\n", i);
        i++;
        //vTaskDelay(pdMS_TO_TICKS(200)); // Delay 1 second
    }
}

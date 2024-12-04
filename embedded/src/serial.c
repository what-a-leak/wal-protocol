#include "serial.h"

#include <stdarg.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int serial_init(int baud_rate, uart_port_t port)
{
    int err = 0;
    // Configure UART parameters
    const uart_config_t uart_config = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
        .source_clk = UART_SCLK_PLL_F80M
    };

    // Install UART driver and set UART pins
    if((err = uart_driver_install(port, 1024, 0, 0, NULL, 0)) != ESP_OK)
        return err;
    err = uart_param_config(port, &uart_config);
    return err;
}

void serial_printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char buff[64] = {0};

    int len = vsprintf(buff, format, args);
    va_end(args);
    uart_write_bytes(UART_NUM_0, buff, len);
}
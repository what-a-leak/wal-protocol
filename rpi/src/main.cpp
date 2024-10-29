#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "uart.hpp"

int main(void)
{
    WAL::Uart uart;
    const char *at = "AT+MODE=TEST\r\n";
    uint8_t *recv_ptr;
    int ret = 0;

    if ((ret = uart.init(UART_BASE_PATH)) < 0)
    {
        printf("Could not establish UART init.\n");
        return ret;
    }

    // Sending AT
    if (uart.send(at, strlen(at)) < 0)
    {
        printf("Failed to send data.");
        return -1;
    }

    // Receiving response from UART
    ssize_t len = uart.recv(recv_ptr);
    if (len < 0)
        printf("UART RX error\n");
    else if (len == 0)
        printf("No response from device\n");
    else
    {
        recv_ptr[len] = '\0';
        printf("(%ld) Received: %s\n", len, recv_ptr);
    }

    return ret;
}

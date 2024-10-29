#include <stdio.h>
#include "uart.hpp"

int main(void)
{
    WAL::Uart uart;
    const char* at = "AT\r\n";
    uint8_t* recv_ptr;
    int ret = 0;

    if ((ret = uart.init("/dev/serial0")) < 0) {
        printf("Could not establish UART init.\n");
        return ret;
    }

    // Sending basic data
    uart.send(at,sizeof(at));
    ssize_t len = uart.recv(recv_ptr);
    if (len < 0) {
        printf("UART RX error\n");
    } else if (len == 0) {
        printf("No response from device\n");
    } else {
        recv_ptr[len] = '\0';  // Null-terminate the received string
        printf("Received: %s\n", recv_ptr);
    }
    
    return ret;
}

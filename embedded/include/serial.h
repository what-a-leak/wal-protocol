#ifndef HEADER_SERIAL
#define HEADER_SERIAL

#include "driver/uart.h"

int serial_init(int baud_rate, uart_port_t port);
void serial_printf(const char* format, ...);

#endif // HEADER_SERIAL
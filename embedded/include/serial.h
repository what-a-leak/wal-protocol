#ifndef HEADER_SERIAL
#define HEADER_SERIAL

int serial_init(int baud_rate);
void serial_printf(const char* format, ...);

#endif // HEADER_SERIAL
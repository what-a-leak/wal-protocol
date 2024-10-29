#include "uart.hpp"
#include <unistd.h>
#include <fcntl.h>

WAL::Uart::Uart()
    : _uart_fs(0)
    , _opt({})
{
    // Baud rate 9600, 8 data bits, no hardware control flow, enable receiver
    _opt.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    // Ignore CR, parity check
    _opt.c_iflag = IGNPAR | IGNCR;
    // No output processing.
    _opt.c_oflag = 0;
    // No line processing.
    _opt.c_lflag = 0;
}

WAL::Uart::~Uart()
{
    close(_uart_fs);
}

int WAL::Uart::init(const std::string &path)
{
    // Read/Write access, No ctrl terminal, //O_NDELAY
    if ((_uart_fs = open(path.c_str(), O_RDWR | O_NOCTTY)) < 0)
        return _uart_fs;

    // Set UART options
    struct termios temp;
    int ret;
    if((ret = tcgetattr(_uart_fs, &temp)) < 0)
        return ret;
    if((ret = tcsetattr(_uart_fs, TCSANOW, &_opt)) < 0)
        return ret;
    return 0;
}

ssize_t WAL::Uart::send(const char* data, size_t len)
{
    return write(_uart_fs, data, len);
}

ssize_t WAL::Uart::recv(uint8_t* &data)
{
    ssize_t read_size = read(_uart_fs, (void*)_rx_buffer, 255);
    data = _rx_buffer;
    return read_size;
}
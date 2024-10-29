#ifndef HEADER_WAL_RPI_UART
#define HEADER_WAL_RPI_UART
#include <termios.h>
#include <string>
#include <cstdint>

#define UART_BASE_PATH  "/dev/serial0"
#define TIMEOUT_SLEEP   30000
namespace WAL
{

    class Uart
    {
        public:
            Uart();
            ~Uart();
            int init(const std::string& path);
            ssize_t send(const char* data, size_t len);
            ssize_t recv(uint8_t* &data, char eol='\n');
        private:
            int _uart_fs;
            struct termios _opt;
            uint8_t _rx_buffer[256];
    };
};

#endif // HEADER_WAL_RPI_UART

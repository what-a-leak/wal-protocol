#ifndef HEADER_LOGGER
#define HEADER_LOGGER

#include <pin.h>
#include <U8g2lib.h>

#define CHAR_LIMIT  24
#define MAX_LOG     6

class Logger{
    public:
        Logger();
        void init();
        void print(const char* str);
    private:
        U8G2_SSD1306_128X64_NONAME_F_HW_I2C _u8g2;
        uint8_t _filled_log;
        char _log[MAX_LOG][CHAR_LIMIT];
};

#endif //HEADER_LOGGER
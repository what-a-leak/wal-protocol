#include "logger.h"

inline static void strcpy_log(char* dst, const char* src)
{
        for (uint8_t i = 0; (src[i] != '\0') || (i < CHAR_LIMIT - 1); i++)
            dst[i] = src[i];
        dst[CHAR_LIMIT] = '\0'; 
}

inline static void copy_log(const char *src, char log[][24], uint8_t pos)
{
    // Adding a new log when empty buffer
    if (pos < MAX_LOG)
        strcpy_log(log[pos],src);
    // Adding a new log when buffer is full
    else
    {
        for(uint8_t i = 1; i<MAX_LOG; i++)
            strcpy(log[i-1], log[i]);
        strcpy_log(log[MAX_LOG-1],src);
    }
}

Logger::Logger()
    : _u8g2(U8G2_R0, U8X8_PIN_NONE, SCREEN_SCL, SCREEN_SDA), _filled_log(0)
{}

void Logger::init()
{
    _u8g2.begin();
    _u8g2.setFont(u8g2_font_busdisplay8x5_tr);
}

void Logger::print(const char *str)
{
    // update the log if filled > MAX_LOG
    copy_log(str, _log, _filled_log);
    // _filled_log (0-6) : filling screen, (7): removing last log, updating
    _filled_log = (_filled_log > MAX_LOG) ? MAX_LOG+1 : _filled_log+1;

    _u8g2.clearBuffer();
    // Loop between the filled log or the max log that could be printed
    for (uint8_t i = 0; (i < MAX_LOG) && (i < _filled_log); i++)
        _u8g2.drawStr(0, (i + 1) * 10, _log[i]);
    _u8g2.sendBuffer();
}

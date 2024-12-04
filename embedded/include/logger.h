#ifndef HEADER_LOGGER
#define HEADER_LOGGER

#include <stdint.h>

#define CHAR_LIMIT  24
#define MAX_LOG     7

void screen_log(const char *format, ...);

#endif //HEADER_LOGGER
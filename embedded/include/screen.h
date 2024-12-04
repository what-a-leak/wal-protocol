#ifndef HEADER_SCREEN
#define HEADER_SCREEN

#include <hal/gpio_types.h>
#include <stdint.h>

void screen_init(gpio_num_t scl, gpio_num_t sda, uint32_t speed);
void screen_draw(const uint8_t line, const char* format, ...);

#endif //HEADER_SCREEN
#ifndef HEADER_BUTTON
#define HEADER_BUTTON

#include <driver/gpio.h>
#include <esp_err.h>

esp_err_t button_init(gpio_num_t pin);
int button_getlevel(void);

#endif //HEADER_BUTTON
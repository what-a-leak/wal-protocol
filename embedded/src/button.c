#include "button.h"

static gpio_num_t _pin = GPIO_NUM_NC;

esp_err_t button_init(gpio_num_t pin)
{
    _pin = pin;
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << _pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    return gpio_config(&io_conf);
}

int button_getlevel(void)
{
    return gpio_get_level(_pin);
}
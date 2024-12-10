#include "button.h"

static gpio_num_t _pin = GPIO_NUM_NC;
static QueueHandle_t _buttonQueue;

static void IRAM_ATTR button_isr_handler(void* arg)
{
    int pin_level = button_getlevel();
    xQueueSendFromISR(_buttonQueue, &pin_level, NULL);
}

esp_err_t button_init(gpio_num_t pin)
{
    _pin = pin;
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << _pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE,
    };
    gpio_install_isr_service(0);
    gpio_isr_handler_add(_pin, button_isr_handler, NULL);
    _buttonQueue = xQueueCreate(2, sizeof(int));
    return gpio_config(&io_conf);
}

int button_getlevel(void)
{
    return gpio_get_level(_pin);
}

QueueHandle_t button_getqueue(void)
{
    return _buttonQueue;
}
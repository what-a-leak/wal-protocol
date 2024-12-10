#ifndef HEADER_BUTTON
#define HEADER_BUTTON

#include <driver/gpio.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

typedef enum{
    NONE,
    PUSH,
    LONG_PUSH,
} ButtonState;

esp_err_t button_init(gpio_num_t pin);
int button_getlevel(void);
QueueHandle_t button_getqueue(void);

#endif //HEADER_BUTTON
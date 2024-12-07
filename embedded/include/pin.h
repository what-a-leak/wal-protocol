#ifndef HEADER_PIN
#define HEADER_PIN

#include <driver/gpio.h>

#define SCREEN_SDA  GPIO_NUM_7
#define SCREEN_SCL  GPIO_NUM_6
#define LORA_TX     GPIO_NUM_16
#define LORA_RX     GPIO_NUM_17

// See wal schematic on https://github.com/what-a-leak/wal-hardware
#define SX1278_PIN_NUM_MISO GPIO_NUM_5
#define SX1278_PIN_NUM_MOSI GPIO_NUM_6
#define SX1278_PIN_NUM_CLK  GPIO_NUM_4
#define SX1278_PIN_NUM_CS   GPIO_NUM_7

#endif //HEADER_PIN
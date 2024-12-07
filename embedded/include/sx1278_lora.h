#ifndef HEADER_SX1278_LORA
#define HEADER_SX1278_LORA

#include "driver/spi_master.h"
#include "driver/gpio.h"

// See wal schematic on https://github.com/what-a-leak/wal-hardware
#define SX1278_PIN_NUM_MISO GPIO_NUM_5
#define SX1278_PIN_NUM_MOSI GPIO_NUM_6
#define SX1278_PIN_NUM_CLK  GPIO_NUM_4
#define SX1278_PIN_NUM_CS   GPIO_NUM_7

// Check SPI communication with SX1278
void check_sx1278(void);

#endif // HEADER_SX1278_LORA

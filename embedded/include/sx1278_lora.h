#ifndef HEADER_SX1278_LORA
#define HEADER_SX1278_LORA

#include <esp_err.h>

// Check SPI communication with SX1278
esp_err_t lora_init(void);
esp_err_t lora_version(uint8_t* major, uint8_t* minor);
esp_err_t lora_clean(void);

#endif // HEADER_SX1278_LORA

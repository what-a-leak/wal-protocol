#ifndef HEADER_SX1278_LORA
#define HEADER_SX1278_LORA

#include <esp_err.h>

// Check SPI communication with SX1278
esp_err_t lora_init(void);
esp_err_t lora_clean(void);

// Not to be used, private function later.
esp_err_t spi_send(uint8_t* tx_data, size_t len);
uint8_t* spi_recv(void);

#endif // HEADER_SX1278_LORA

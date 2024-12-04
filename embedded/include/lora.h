#ifndef HEADER_LORA
#define HEADER_LORA

#include <esp_err.h>

esp_err_t lora_init();
esp_err_t lora_test();
esp_err_t lora_config();
esp_err_t lora_setfrequency(uint16_t frq_mhz);

#endif // HEADER_LORA
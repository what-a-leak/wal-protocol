#ifndef HEADER_SX1278_LORA
#define HEADER_SX1278_LORA

#include <esp_err.h>

#define DEBUG_WAL

#ifdef DEBUG_WAL
#include "logger.h"
#define WAL_PRINT(...)  screen_log(__VA_ARGS__)   
#else
#define WAL_PRINT(...)
#endif

// Init and Clean functions
esp_err_t lora_init(void);
esp_err_t lora_clean(void);

// Modifying registers
esp_err_t lora_set_frequency(long freq);
esp_err_t lora_enable_crc();
esp_err_t lora_set_coding_rate(int denominator);
esp_err_t lora_set_bandwidth(int bandwidth);
esp_err_t lora_set_spreading_factor(int spreading_factor);

// Getters for registers
int lora_get_coding_rate(void);
int lora_get_bandwidth(void);
int lora_get_spreading_factor(void);

// Internal function: will be private soon
void lora_reset();
void lora_set_tx_power(int level);
esp_err_t lora_version(uint8_t* major, uint8_t* minor);

#endif // HEADER_SX1278_LORA

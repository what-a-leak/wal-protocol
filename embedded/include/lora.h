#ifndef HEADER_LORA
#define HEADER_LORA

#include <esp_err.h>

typedef enum {
        INIT_MODE = 0,  // Initialising
        SLEEP_MODE,     // Low power sleep mode (if supported)
        IDLE_MODE,      // Idle
        TX_MODE,        // Transmitting
        RX_MODE         // Receiving
} lora_mode_t;

esp_err_t lora_init();
esp_err_t lora_test();
esp_err_t lora_config();
esp_err_t lora_setfrequency(uint16_t frq_mhz);
int lora_available();
esp_err_t lora_send(uint8_t* data, uint8_t len);
esp_err_t lora_recv(uint8_t* buf, uint8_t* len);

#endif // HEADER_LORA
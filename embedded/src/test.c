#include "test.h"
#include "logger.h"

#define CODING_RATE         1
#define BANDWIDTH           7
#define SPREADING_FACTOR    7

uint8_t _setup = 1;

int test_recv_setup()
{
    screen_log("LoRa:recv_setup");
    lora_init();
    lora_set_frequency(433);
    lora_enable_crc();
    lora_set_coding_rate(CODING_RATE);
    screen_log("LoRa: cr= %d", lora_get_coding_rate());
    lora_set_bandwidth(BANDWIDTH);
    screen_log("LoRa: bw= %d", lora_get_bandwidth());
    lora_set_spreading_factor(SPREADING_FACTOR);
    screen_log("LoRa: sf= %d", lora_get_spreading_factor());
    

    return 0;
}

int test_recv_loop()
{
    if(_setup)
    {
        screen_log("LoRa:recv_loop");
        _setup = 0;
    }
    return 0;
}

int test_send_setup()
{
    screen_log("LoRa:send_setup");
    return 0;
}

int test_send_loop()
{
    if(_setup)
    {
        screen_log("LoRa:send_loop");
        _setup = 0;
    }
    return 0;
}


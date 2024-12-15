#include "test.h"
#include "logger.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define CODING_RATE         8   // CR = 4/8
#define BANDWIDTH           6   // 62.5 kHz from the datasheet
#define SPREADING_FACTOR    12  // 4096 chirps: SF12 
#define MAX_PACKET_SIZE     256

typedef enum send_mode_e {
    CONTINUOUS,
    SINGLE
} SendMode;

static uint32_t     _count = 0;
static uint8_t      _rx_buff[MAX_PACKET_SIZE] = {0};
static SendMode     _mode = CONTINUOUS;
static const char* _packet = "Hello World!";

inline static void test_setup()
{
    screen_log("LoRa:recv_setup");
    lora_init();
    lora_set_frequency(433e6);
    screen_log("LoRa:f= %0.0f MHz", lora_get_frequency()/1e6);
    lora_enable_crc();
    lora_set_coding_rate(CODING_RATE);
    screen_log("LoRa:cr= %d", lora_get_coding_rate());
    lora_set_bandwidth(BANDWIDTH);
    screen_log("LoRa:bw= %d", lora_get_bandwidth());
    lora_set_spreading_factor(SPREADING_FACTOR);
    screen_log("LoRa:sf= %d", lora_get_spreading_factor());
}

int test_recv_setup()
{
    test_setup();
    // Set the mode to receive
    lora_receive();
    screen_log("LoRa:RX Mode");
    return 0;
}

int test_recv_loop()
{
    if (lora_received())
    {
        const int len = lora_receive_packet(_rx_buff, MAX_PACKET_SIZE);
        const int rssi = lora_packet_rssi();
        screen_log("LoRa:[%ld]rx %d", _count, len);
        screen_log("   %d dB", rssi);
        printf("[%ld] LoRa: received packet of size %d!\n", _count, len);
        _count++;
    }
    return 0;
}

int test_send_setup()
{
    test_setup();
    screen_log("LoRa:send_setup");
    return 0;
}

void test_change_mode(ButtonState button)
{
    if(button == LONG_PUSH)
    {
        _mode = 1-_mode;
        if(_mode == CONTINUOUS)
            screen_log("mode:TX loop");
        else
            screen_log("mode:TX single");
    }
}

int test_send_loop(ButtonState button)
{
    /* Sending packet depending on the mode */
    esp_err_t err = ESP_OK;
    if((_mode == CONTINUOUS) ||(_mode == SINGLE && button == PUSH))
        err = lora_send_packet(_packet, 13);
    else
        return 0;

    /* Checking if sending is successfull */
    if(err != ESP_OK)
    {
        screen_log("LoRa:[%ld]tx NO", _count);
        printf("[%ld] LoRa: failed to send packet!\n", _count);
        _count++;
    }
    else
    {
        screen_log("LoRa:[%ld]tx OK", _count);
        printf("[%ld] LoRa: send packet of size %d: %s!\n", _count, 13, _packet);
        _count++;
    }

    /* If continous -> wait for next payload */
    if(_mode == CONTINUOUS)
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    return 0;
}


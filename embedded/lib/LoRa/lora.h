#ifndef HEADER_LORA
#define HEADER_LORA
#include <HardwareSerial.h>

struct LoRaHeader {
    uint8_t id;
    uint8_t addrTo;
    uint8_t flags;
};

class LoRa {
    public:
        LoRa(uint8_t addr=0xff);
        bool init();
        bool setFrequency(uint16_t frq_mhz);
        bool send(LoRaHeader header, uint8_t* data, uint8_t len);
    private:
        uint8_t _addr;
        uint16_t _len_preamble;
        HardwareSerial _serial;
};

#endif // HEADER_LORA
#include "lora.h"

#include <pin.h>
#include "rf95.h"

/*
#include <logger.h>
extern Logger debug_log;
*/

static inline void write(HardwareSerial &serial, uint8_t reg, uint8_t val)
{
    const char data[4] = {'W', static_cast<uint8_t>(reg | RH_WRITE_MASK), 1, val};
    serial.write(data, 4);
}

static inline int16_t read(HardwareSerial &serial, uint8_t reg)
{
    const char data[3] = {'R', static_cast<uint8_t>(reg & ~RH_WRITE_MASK), 1};
    int16_t val = -1;
    unsigned long timerStart, timerEnd;

    serial.write(data, 3);
    timerStart = millis();
    while (1)
    {
        if (serial.available()) {
            val = static_cast<int16_t>(serial.read());
            break;
        }

        timerEnd = millis();
        if (timerEnd - timerStart > 1000 * DEFAULT_TIMEOUT)
            break;
    }  
    return val;
}

LoRa::LoRa(uint8_t addr)
    : _addr(addr)
    , _len_preamble(8)
    , _serial(1)
{
}

bool LoRa::init()
{
    _serial.begin(57600, SERIAL_8N1, LORA_RX, LORA_TX);
    // Setting to Mode Sleep -> Only then we can set to Long Range (Lora Mode).
    write(_serial, RH_RF95_REG_01_OP_MODE, RH_RF95_LONG_RANGE_MODE | RH_RF95_MODE_SLEEP);
    // Using the full FIFO 256 bytes.
    write(_serial, RH_RF95_REG_0E_FIFO_TX_BASE_ADDR, 0);
    write(_serial, RH_RF95_REG_0F_FIFO_RX_BASE_ADDR, 0);
    // Setting preamble length
    write(_serial, RH_RF95_REG_20_PREAMBLE_MSB, _len_preamble >> 8);
    write(_serial, RH_RF95_REG_21_PREAMBLE_LSB, _len_preamble & 0xff);
    // Verifying that the register has been correctly modified.
    return (
        (read(_serial, RH_RF95_REG_01_OP_MODE) == (RH_RF95_MODE_SLEEP | RH_RF95_LONG_RANGE_MODE))
        && (read(_serial,RH_RF95_REG_0E_FIFO_TX_BASE_ADDR) == 0)
        && (read(_serial,RH_RF95_REG_0F_FIFO_RX_BASE_ADDR) == 0)
        && (read(_serial,RH_RF95_REG_20_PREAMBLE_MSB) == (_len_preamble >> 8))
        && (read(_serial,RH_RF95_REG_21_PREAMBLE_LSB) == (_len_preamble & 0xff))
        );
}

bool LoRa::setFrequency(uint16_t frq_mhz)
{
    // 1. converting MHz to Hz
    // 2. f/f_RES = fRF
    uint32_t frf = (frq_mhz * 1000000.0) / F_RES;
    write(_serial, RH_RF95_REG_06_FRF_MSB, (frf >> 16) & 0xff);
    write(_serial, RH_RF95_REG_07_FRF_MID, (frf >> 8) & 0xff);
    write(_serial, RH_RF95_REG_08_FRF_LSB, frf & 0xff);

    uint32_t res =
        (static_cast<uint32_t>(read(_serial, RH_RF95_REG_06_FRF_MSB) << 16)) + (static_cast<uint16_t>(read(_serial, RH_RF95_REG_07_FRF_MID) << 8)) + read(_serial, RH_RF95_REG_08_FRF_LSB);
    return (res == frf);
}

bool LoRa::send(LoRaHeader header, uint8_t *data, uint8_t len)
{
}
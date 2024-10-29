#include <logger.h>
#include <lora.h>

Logger debug_log;
static LoRa lora;
static char recv_buf[512];

void setup(void)
{
    debug_log.init();
    debug_log.print("[OK] Screen init.");

    debug_log.print("Enabling LoRa...");
    if(lora.init())
        debug_log.print("[OK] LoRa init.");
    else
        debug_log.print("[ERR] LoRa not found.");

    if(lora.setFrequency(433))
        debug_log.print("[OK] Freq 433 MHz.");
    else
        debug_log.print("[ERR] Freq not init.");
}

void loop(void)
{
}
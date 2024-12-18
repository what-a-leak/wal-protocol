#include "packet.h"

wal_payload_header_t _header = {0};

static inline void packet_init(void)
{
    _header.header.name = WAL_NAME;
    _header.header.ver_major = WAL_MAJOR;
    _header.header.ver_minor = WAL_MINOR;
}

void packet_init_full_addr(uint32_t src, uint32_t dst)
{
    packet_init();
    _header.header.addr_size = WAL_ADDR_FULL;
}

void packet_init_min_addr(uint8_t src, uint8_t dst)
{
    packet_init();
    _header.header.addr_size = WAL_ADDR_MIN;
}

/* Setters */
void packet_set_leak(int status) { _header.command.has_leak = status; }

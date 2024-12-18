#ifndef HEADER_WAL_PROTOCOL_PACKET
#define HEADER_WAL_PROTOCOL_PACKET

#include "structure.h"

void packet_init_full_addr(uint32_t src, uint32_t dst);
void packet_init_min_addr(uint8_t src, uint8_t dst);


void packet_set_leak(int status);

#endif //HEADER_WAL_PROTOCOL_PACKET
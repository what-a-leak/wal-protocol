#ifndef HEADER_WAL_PROTOCOL_PACKET
#define HEADER_WAL_PROTOCOL_PACKET

#include <stddef.h>

#include "structure.h"

/* ------------ */
/* Initializers */
/* ------------ */
void packet_init_full_addr(uint32_t addr_dst);
void packet_init_min_addr(uint8_t addr_dst);

/* ---------------- */
/* Creating packets */
/* ---------------- */

/**
 * @brief Add a data payload with its enderlying type and size.
 * The data should already be allocated prior to this function,
 * no deallocation will be done, only the pointer gets referenced and
 * recasted to the type given.
 * 
 * @param type enumeration of all the types that can be forwarded
 * @param data the pointer to the data
 * @param size the size of the data (maximum: 2048)
 */
void packet_add_data(wal_type_t type, wal_label_t label, void* data, size_t size);

uint8_t* packet_get(size_t* size);
wal_error_t packet_parse(uint8_t* data, uint8_t* cmd, uint8_t** msg);

/* ------------------ */
/* Converting packets */
/* ------------------ */
wal_payload_data_t packet_parse_data(uint8_t* msg);

/* ------------ */
/* Misc & Debug */
/* ------------ */
void packet_set_leak(int status);
void packet_set_src_addr(uint32_t addr);

#endif //HEADER_WAL_PROTOCOL_PACKET
#ifndef HEADER_WAL_PROTOCOL
#define HEADER_WAL_PROTOCOL

#include <stdint.h>

#define WAL_NAME        0b011010100
#define WAL_MAJOR       1
#define WAL_MINOR       0

#define WAL_ADDR_FULL   0
#define WAL_ADDR_MIN    1

typedef struct __attribute__((packed)) wal_header_s {
    uint16_t name:      9;      // should be equal to 0xd4
    uint8_t ver_major:  7;
    uint8_t ver_minor:  7;
    uint8_t addr_size:  1;
} wal_header_t;

#define WAL_CMD_TOPOLOGY    0
#define WAL_CMD_TIME        1
#define WAL_CMD_DATA        2
#define WAL_CMD_BATTERY     3

typedef struct __attribute__((packed)) wal_command_s {
    uint8_t cmd:        4;
    uint8_t has_leak:   1;
} wal_command_t;

typedef struct __attribute__((packed)) wal_payload_header_s {
    wal_header_t header;
    uint32_t addr_src;
    uint32_t addr_dst;
    wal_command_t command;
} wal_payload_header_t;

/*
    For now: Topology, Time and Battery payloads are ignored.
    They will be done shortly. We will mainly focus on data.
*/

#define WAL_TYPE_UINT8      0
#define WAL_TYPE_UINT16     0b00001
#define WAL_TYPE_UINT32     0b00010
#define WAL_TYPE_UINT64     0b00011
#define WAL_TYPE_FLOAT32    0b00100
#define WAL_TYPE_SIGNED     0x10

#define WAL_TYPE_MAXSIZE    ((1 << 11)-1)

typedef struct __attribute__((packed)) wal_type_s {
    uint8_t type:       5;
    uint16_t size:      11;
} wal_type_t;

typedef struct __attribute__((packed)) wal_payload_data_s {
    wal_type_t header;
    void*   data;
} wal_payload_data_t;

#endif //HEADER_WAL_PROTOCOL
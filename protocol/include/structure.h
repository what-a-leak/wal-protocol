#ifndef HEADER_WAL_PROTOCOL_STRUCTURE
#define HEADER_WAL_PROTOCOL_STRUCTURE

#include <stdint.h>

#define WAL_NAME        0xd4//0b011010100
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

enum wal_command_e {
    WAL_CMD_TOPOLOGY,
    WAL_CMD_TIME,
    WAL_CMD_DATA,
    WAL_CMD_BATTERY,
    WAL_CMD_MAX = (1 << 4),
};

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

typedef enum wal_type_e {
    WAL_TYPE_UINT8,
    WAL_TYPE_UINT16,
    WAL_TYPE_UINT32,
    WAL_TYPE_UINT64,
    WAL_TYPE_FLOAT32,
    WAL_TYPE_SIGNED = (1 << 4),
} wal_type_t;

#define WAL_TYPE_MAXSIZE    ((1 << 11)-1)

typedef struct __attribute__((packed)) wal_data_header_s {
    uint8_t type:       5;
    uint16_t size:      11;
} wal_data_header_t;

typedef enum wal_label_e {
    WAL_LABEL_NULL,
    WAL_LABEL_FFT,
    WAL_LABEL_MICROPHONE,
} wal_label_t;

typedef struct __attribute__((packed)) wal_payload_data_s {
    wal_data_header_t header;
    uint8_t label;
    void*   data;
} wal_payload_data_t;

#endif //HEADER_WAL_PROTOCOL_STRUCTURE
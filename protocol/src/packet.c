#include "packet.h"

#include <stdio.h>

/* ------- */
/* private */
/* ------- */

wal_payload_header_t _header = {0};
wal_payload_data_t _payload_data = {0};
uint8_t _packet[256];  // 256 bytes, enough for now.

static inline void packet_init(void)
{
    _header.header.name = WAL_NAME;
    _header.header.ver_major = WAL_MAJOR;
    _header.header.ver_minor = WAL_MINOR;
}

// ! UNUSED
static inline size_t packet_size(void)
{
    size_t size = sizeof(wal_header_t);
    
    /* Defining address size */
    if(_header.header.addr_size == WAL_ADDR_FULL)
        size += 6;
    else if (_header.header.addr_size == WAL_ADDR_MIN)
        size += 2;
    size += sizeof(wal_command_t);
    
    /* Defining the next payload size with the command */
    switch(_header.command.cmd)
    {
        case WAL_CMD_DATA:
            size += sizeof(wal_data_header_t);
            size += _payload_data.header.size;
            break;
        default:
            break;
    }
    return size;
}

/* ------ */
/* public */
/* ------ */

void packet_init_full_addr(uint32_t src, uint32_t dst)
{
    packet_init();
    _header.header.addr_size = WAL_ADDR_FULL;
    _header.addr_src = src;
    _header.addr_dst = dst;
}

void packet_init_min_addr(uint8_t src, uint8_t dst)
{
    packet_init();
    _header.header.addr_size = WAL_ADDR_MIN;
    _header.addr_src = src;
    _header.addr_dst = dst;
}

void packet_add_data(wal_type_t type, wal_label_t label, void* data, size_t size)
{
    _header.command.cmd = (uint8_t)WAL_CMD_DATA;
    if(size > WAL_TYPE_MAXSIZE)
        size = WAL_TYPE_MAXSIZE;
    _payload_data.header.size = size;
    _payload_data.header.type = (uint8_t)type;
    _payload_data.label = (uint8_t)label;
    _payload_data.data = data;
}

uint8_t* packet_get(size_t* size)
{
    size_t offset = 0;

    /* Original HEADER */
    _packet[offset]  = _header.header.name >> 1;
    offset++;
    _packet[offset] = _header.header.name << 7;
    _packet[offset] |= _header.header.ver_major;
    offset++;
    _packet[offset] = _header.header.ver_minor << 1;
    _packet[offset] |= _header.header.addr_size;
    offset++;

    /* Address */
    if(_header.header.addr_size == WAL_ADDR_FULL)
    {
        _packet[offset] = (_header.addr_src >> 16);
        _packet[offset+1] = (_header.addr_src >> 8);
        _packet[offset+2] = (_header.addr_src >> 0);
        _packet[offset+3] = (_header.addr_dst >> 16);
        _packet[offset+4] = (_header.addr_dst >> 8);
        _packet[offset+5] = (_header.addr_dst >> 0);
        offset += 6;
    }
    else {
        _packet[offset] = _header.addr_src;
        _packet[offset+1] = _header.addr_dst;
        offset += 2;
    }
    /* Command */
    _packet[offset] = _header.command.cmd << 4;
    _packet[offset] |= _header.command.has_leak << 3;
    offset++;

    /* Depending on the given command */
    switch(_header.command.cmd)
    {
        case WAL_CMD_DATA:
            _packet[offset] = _payload_data.header.type << 3;
            _packet[offset] |= _payload_data.header.size >> 8;
            offset++;
            _packet[offset] = _payload_data.header.size & 0xff;
            offset++;
            _packet[offset] = _payload_data.label;
            // offset += _payload_data.header.size;
            break;
        default:
            break;
    }

    *size = offset;
    return _packet;
}

/* ------- */
/* setters */
/* ------- */

void packet_set_leak(int status) { _header.command.has_leak = status; }

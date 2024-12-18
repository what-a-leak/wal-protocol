#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "packet.h"

#define SRC_ADDR    0x123456
#define DST_ADDR    0x789abc

int main(void)
{
    uint8_t data[11] = {0xff,11,12,13,14,15,16,17,18,19,20};
    uint8_t* packet = NULL;
    size_t packet_size = 0;

    printf("Creating packet...");
    packet_set_src_addr(SRC_ADDR);
    packet_init_full_addr(DST_ADDR);
    packet_add_data(WAL_TYPE_UINT8, WAL_LABEL_FFT, data, 11);
    packet = packet_get(&packet_size);
    // Just for testing purposes, we add the rest of the data
    memcpy(packet+packet_size,data,11*sizeof(uint8_t));
    packet_size+=11;

    // Printing the data
    for(size_t i=0; i<packet_size; i++)
        printf("0x%x ", packet[i]);
    printf("\n");

    // Testing the parser
    uint8_t *msg, cmd;
    wal_payload_data_t p_data;
    packet_set_src_addr(DST_ADDR);
    packet_parse(packet, &cmd, &msg);
    if(cmd == WAL_CMD_DATA)
    {
        printf("Successfully gathered Data packet!\n");
        p_data = packet_parse_data(msg);
        if(p_data.header.type == WAL_TYPE_UINT8)
        {
            printf("uint8 type data OK! Allocating with size: %d\n", p_data.header.size);
            uint8_t* g_data = (uint8_t*)malloc(sizeof(uint8_t)*p_data.header.size);
            memcpy(g_data, p_data.data, sizeof(uint8_t)*p_data.header.size);
            printf("Gathered: ");
            for(size_t i=0; i<p_data.header.size; i++)
                printf("0x%x ", g_data[i]);
            printf("\n");
        }
    }
    return 0;
}

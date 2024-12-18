#include <stdio.h>

#include "packet.h"

int main(void)
{
    uint8_t data[11] = {0xff,11,12,13,14,15,16,17,18,19,20};
    uint8_t* packet = NULL;
    size_t packet_size = 0;

    printf("Creating packet...");
    packet_init_full_addr(0x123456,0x789abc);
    packet_add_data(WAL_TYPE_UINT8, WAL_LABEL_FFT, data, 11);
    packet = packet_get(&packet_size);
    for(size_t i=0; i<packet_size; i++)
        printf("0x%x ", packet[i]);
    for(size_t i=0; i<11; i++)
        printf("0x%x ", data[i]);
    printf("\n");
    return 0;
}
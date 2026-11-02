#include <iso646.h>
#include <stdlib.h>
#include "ch_bytes.h"
#include "../include/ch_test.h"

int main(void) {
    unsigned char* dst_16 = malloc(2);
    unsigned char* dst_32 = malloc(4);
    unsigned char* dst_64 = malloc(8);


    if (dst_16 == NULL || dst_32 == NULL || dst_64 == NULL) {return 1;}  // otazka na Filipa: je chyba nespravit ifko na to, aby sa checkla alokacia pamate? chyba to asi urcite je ale ci sa to riesi minusovym bodom :D

    const uint16_t value_16 = 0xABCD;
    const uint32_t value_32 = 0x12345678;
    const uint64_t value_64 = 0x123456789ABCDEF0ULL; // ULL je unsigned long long aby ho kompilator neprekonvertoval na 32bit int

    ch_put_u16(dst_16, value_16);
    ch_put_u32(dst_32, value_32);
    ch_put_u64(dst_64, value_64);


    CH_CHECK(dst_16[0] == 0xAB);
    CH_CHECK(dst_16[1] == 0xCD);
    CH_CHECK(ch_get_u16(dst_16) == value_16);

    CH_CHECK(dst_32[0] == 0x12);
    CH_CHECK(dst_32[1] == 0x34);
    CH_CHECK(dst_32[2] == 0x56);
    CH_CHECK(dst_32[3] == 0x78);
    CH_CHECK(ch_get_u32(dst_32) == value_32);


    CH_CHECK(dst_64[0] == 0x12);
    CH_CHECK(dst_64[7] == 0xF0);
    CH_CHECK(ch_get_u64(dst_64) == value_64);


    free(dst_16);
    free(dst_32);
    free(dst_64);
    return ch_test_result();
}

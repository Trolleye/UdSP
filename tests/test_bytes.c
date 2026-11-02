#include <stdlib.h>
#include "ch_bytes.h"
#include "../include/ch_test.h"

int main(void) {
    unsigned char* dst = malloc(2);
    if (dst == NULL) {return 1;}  // otazka na Filipa: je chyba nespravit ifko na to, aby sa checkla alokacia pamate? chyba to asi urcite je ale ci sa to riesi minusovym bodom :D
    const uint16_t value = 0xABCD;

    ch_put_u16(dst, value);

    CH_CHECK(dst[0] == 0xAB);
    CH_CHECK(dst[1] == 0xCD);
    CH_CHECK(ch_get_u16(dst) == value);

    free(dst);

    return ch_test_result();
}

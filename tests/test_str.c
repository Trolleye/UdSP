#include "../include/ch_test.h"
#include <assert.h>
#include "../include/ch_str.h"
#include "../include/ch_test.h"
#include <stdlib.h>
#include <string.h>


int main(void) {
    CH_CHECK(ch_str_len("") == 0);
    CH_CHECK(ch_str_len("Ahoj") == 4);

    const char* s = "Žilina";
    CH_CHECK(ch_str_len(s) == 7);
    CH_CHECK(ch_utf8_count(s) == 6);

    return ch_test_result();
}

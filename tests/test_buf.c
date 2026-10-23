#include <string.h>

#include "ch_buf.h"
#include "ch_test.h"

int main(void) {
    ch_buf_cleanup(NULL);

    ch_buf buf;
    ch_buf *b = &buf;
    CH_CHECK(ch_buf_init(b, 0) == CH_OK);
    CH_CHECK(ch_buf_size(b) == 0);
    CH_CHECK(ch_buf_capacity(b) > 0);

    CH_CHECK(ch_buf_append_str(b, "Ahoj") == CH_OK);
    CH_CHECK(ch_buf_size(b) == 4);
    CH_CHECK(memcmp(ch_buf_data(b), "Ahoj", 4) == 0);

    CH_CHECK(ch_buf_append_str(b, " svet") == CH_OK);
    CH_CHECK(ch_buf_size(b) == 9);
    CH_CHECK(memcmp(ch_buf_data(b), "Ahoj svet", 9) == 0);

    const size_t before = ch_buf_capacity(b);
    for (int i = 0; i < 1000; i++)
        CH_CHECK(ch_buf_append_str(b, "0123456789") == CH_OK);
    CH_CHECK(ch_buf_size(b) == 9 + 10000);
    CH_CHECK(ch_buf_capacity(b) > before);
    CH_CHECK(memcmp(ch_buf_data(b), "Ahoj svet0123456789", 19) == 0);

}

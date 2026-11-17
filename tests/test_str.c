#include "../include/ch_test.h"
#include "../include/ch_str.h"
#include <string.h>


int main(void) {
    CH_CHECK(ch_b64_encoded_len(0) == 0);
    CH_CHECK(ch_b64_encoded_len(3) == 4);
    CH_CHECK(ch_b64_encoded_len(6) == 8);

    char* c = "aA+";
    char encoded[8];
    CH_CHECK(ch_b64_encode(c, 3, encoded, sizeof(encoded)) == CH_OK);
    CH_CHECK(strcmp(encoded, "YUEr") == 0);

    //test paddingu 1B
    CH_CHECK(ch_b64_encode("a", 1, encoded, sizeof(encoded)) == CH_OK);
    CH_CHECK(strcmp(encoded, "YQ==") == 0);

    //test paddingu 2B
    CH_CHECK(ch_b64_encode("aA", 2, encoded, sizeof(encoded)) == CH_OK);
    CH_CHECK(strcmp(encoded, "YUE=") == 0);

    char small_buf[2];
    CH_CHECK(ch_b64_encode(c, 3, small_buf, sizeof(small_buf)) == CH_ERR_RANGE);

    unsigned char decoded[8];
    size_t decoded_len = 0;

    CH_CHECK(ch_b64_decode("YUEr", decoded, sizeof(decoded), &decoded_len) == CH_OK);
    CH_CHECK(decoded_len == 3);
    CH_CHECK(memcmp(decoded, "aA+", 3) == 0);

    CH_CHECK(ch_b64_decode("YQ==", decoded, sizeof(decoded), &decoded_len) == CH_OK);
    CH_CHECK(decoded_len == 1);
    CH_CHECK(decoded[0] == 'a');

    CH_CHECK(ch_b64_decode("YUE=", decoded, sizeof(decoded), &decoded_len) == CH_OK);
    CH_CHECK(decoded_len == 2);
    CH_CHECK(memcmp(decoded, "aA", 2) == 0);

    CH_CHECK(ch_b64_decode("YUE", decoded, sizeof(decoded), &decoded_len) == CH_ERR_FORMAT);  // text musi byt delitelny 4
    CH_CHECK(ch_b64_decode("YUE-", decoded, sizeof(decoded), &decoded_len) == CH_ERR_FORMAT); // neplatny znak -
    CH_CHECK(ch_b64_decode(NULL, decoded, sizeof(decoded), &decoded_len) == CH_ERR_ARG);

    return ch_test_result();
}
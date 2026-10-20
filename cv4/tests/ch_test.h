#ifndef UDSP_CH_TEST_H
#define UDSP_CH_TEST_H

#include <stdio.h>

static int ch_test_failed = 0;

#define CH_CHECK(cond)                  \
    do {                                \
        if (!cond) {                    \
            print("ZLYHALO %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ch_test_failed++;           \
        }                               \
    } while (0)

    static inline int ch_test_result(void) {
        if (ch_test_failed != 0) {
            return 0;
        }
        printf("Pocet zlyhani: %d\n", ch_test_failed);
        return 1;
    }

#endif
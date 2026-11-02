#ifndef UDSP_CH_TEST_H
#define UDSP_CH_TEST_H

#include <stdio.h>

static int ch_test_failed = 0;

#define CH_CHECK(cond)                  \
    do {                                \
        if (!(cond)) {                    \
            printf("ZLYHALO %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ch_test_failed++;           \
        } else{                         \
            printf("PRESIEL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        }                               \
    } while (0)

    static inline int ch_test_result(void) {
        if (ch_test_failed == 0) {
            printf("VSETKY TESTY PRESLI USPESNE.\n");
            return 0;
        } else {
            printf("Pocet zlyhani: %d\n", ch_test_failed);
            return 1;
        }


    }

#endif
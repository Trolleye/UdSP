//
// Created by vorcak8 on 27. 10. 2026.
//

#include "../include/ch_hex.h"

#include <stdint.h>
#define CH_HEX_COLS 16


void ch_hexdump(const void* data, size_t size) {
    ch_hexdump_to(stdout, NULL, data, size);
}

static char is_printable(unsigned char c) {
    return (c >= 0x20 && c < 0x7f) ? (char)c : '.';
}

void ch_hexdump_to(FILE* out, const char* label, const void* data, size_t size) {
    if (out == NULL) {
        return;
    }

    if (label == NULL) {
        fprintf(out, "%s (%zu)\n", label, size);
    }

    if (data == NULL || size == 0) {
        fprintf(out, " <prazdny blok>\n");
        return;
    }

    const unsigned char* p = (const unsigned char*)data;

    for (size_t off = 0; off < size; off += CH_HEX_COLS) {
        fprintf(out, "  %08zX  ", off);
        for (size_t i = 0; i < CH_HEX_COLS; i++) {
            if (off + i < size) {
                fprintf(out, "%02X   ", p[off + i]);
            }   else {
                fprintf(out, "\t");
            }

            if (i == CH_HEX_COLS / 2 - 1) {
                fputc(' ', out);
            }

        }

        fprintf(out, " |");
        for (size_t i = 0; i < CH_HEX_COLS && off + i < size; i++) {
            fputc(is_printable(p[off + i]), out);
        }
        fprintf(out, "\n");
    }


}
//
// Created by vorcak8 on 27. 10. 2026.
//

#include "../include/ch_flags.h"

#include <stdio.h>

ui32 ch_flags_set(ui32 flags, ui32 which) { return flags | which; }
ui32 ch_flags_clear(ui32 flags, ui32 which) { return flags & ~which; }
ui32 ch_flags_toggle(ui32 flags, ui32 which) { return flags ^ which; }
int  ch_flags_test(ui32 flags, ui32 which) { return (flags & which) == which;  }
int  ch_flags_count(ui32 flags) {
    int n = 0;
    while (flags != 0u) {
        flags &= flags - 1u;
        ++n;
    }
    return n;
}
void ch_flag_describe(ui32 flags, char* restrict buf, size_t size) {
    static const struct {
        ui32 bit;
        const char* name;
    } table [] = {
        {CH_FLAG_URGENT, "URGENT"},
        {CH_FLAG_SYSTEM, "SYSTEM"},
        {CH_FLAG_ENCRYPTED, "ENCRYPTED"},
        {CH_FLAG_COMPRESSED, "COMPRESSED"},
        {CH_FLAG_HAS_BODY, "HAS_BODY"}
    };

    if (buf == NULL || size == 0) return;
    buf[0] = '\0';
    size_t used = 0;
    for (size_t i = 0; i < sizeof(table) / sizeof(table[0]); ++i) {
        if (!ch_flags_test(flags, table[i].bit)) continue;
        const char* separator = (used > 0) ? "|" : "";
        int n = snprintf(buf + used, size - used, "%s%s", separator, table[i].name);
        if (n < 0 || (size_t)n >= size - used) return;
        used += (size_t)n;
    }
    if (used == 0) snprintf(buf, size, "NONE");
}
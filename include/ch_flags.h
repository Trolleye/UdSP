//
// Created by vorcak8 on 27. 10. 2026.
//

#ifndef CV4_CH_FLAGS_H
#define CV4_CH_FLAGS_H

#include <stdint.h>
#include <stddef.h>
typedef uint32_t ui32;

enum {
    CH_FLAG_NONE        = 0u,
    CH_FLAG_URGENT      = 1u << 0,
    CH_FLAG_SYSTEM      = 1u << 1,
    CH_FLAG_ENCRYPTED   = 1u << 2,
    CH_FLAG_COMPRESSED  = 1u << 3,
    CH_FLAG_HAS_BODY    = 1u << 4,

    CH_FLAG_ALL = CH_FLAG_URGENT | CH_FLAG_SYSTEM | CH_FLAG_ENCRYPTED | CH_FLAG_COMPRESSED | CH_FLAG_HAS_BODY
};

ui32 ch_flags_set(ui32 flags, ui32 which);
ui32 ch_flags_clear(ui32 flags, ui32 which);
ui32 ch_flags_toggle(ui32 flags, ui32 which);
int  ch_flags_test(ui32 flags, ui32 which);
int  ch_flags_count(ui32 flags);

void ch_flags_describe(ui32 flags, char* restrict buf, size_t size);


#endif //CV4_CH_FLAGS_H

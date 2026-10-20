
#ifndef CH_STR_H
#define CH_STR_H

#include <stddef.h>

#include "ch_err.h"

size_t ch_str_len(const char *s);

ch_err ch_str_copy(char *restrict dst, size_t size,
                   const char *restrict src);

char *ch_str_dup(const char *src);

void ch_str_trim(char *s);

size_t ch_str_split(char *s, char **out, size_t max);


#endif /* CH_STR_H */

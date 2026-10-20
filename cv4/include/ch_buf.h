
#ifndef CH_BUF_H
#define CH_BUF_H

#include <stddef.h>

#include "../ch_err.h"

typedef struct ch_buf {
    unsigned char *data;
    size_t         size;
    size_t         capacity;
} ch_buf;

ch_err ch_buf_init(ch_buf *self, size_t capacity);

void ch_buf_cleanup(ch_buf *self);

ch_err ch_buf_reserve(ch_buf *self, size_t need);

ch_err ch_buf_append(ch_buf *self, const void *restrict data, size_t size);


ch_err ch_buf_append_str(ch_buf *self, const char *restrict text);


void ch_buf_consume(ch_buf *self, size_t n);


void ch_buf_clear(ch_buf *self);

size_t               ch_buf_size(const ch_buf *self);
size_t               ch_buf_capacity(const ch_buf *self);
const unsigned char *ch_buf_data(const ch_buf *self);

#endif /* CH_BUF_H */

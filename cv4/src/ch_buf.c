#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../include/ch_buf.h"

#define CH_BUF_DEFAULT 64

ch_err ch_buf_init(ch_buf *self, size_t capacity)
{
    assert(self != NULL);

    if (capacity == 0)
        capacity = CH_BUF_DEFAULT;

    self->data = malloc(capacity);
    if (self->data == NULL)
        return CH_ERR_MEM;

    self->size     = 0;
    self->capacity = capacity;
    return CH_OK;
}

void ch_buf_cleanup(ch_buf *self)
{
    if (self == NULL)
        return;

    free(self->data);
    self->data     = NULL;
    self->size     = 0;
    self->capacity = 0;
}

ch_err ch_buf_reserve(ch_buf *self, size_t need)
{
    assert(self != NULL);
    assert(self->data != NULL);

    if (need <= self->capacity - self->size)
        return CH_OK;

    size_t want = self->size + need;
    if (want < self->size)
        return CH_ERR_RANGE;

    size_t capacity = self->capacity;
    while (capacity < want) {
        if (capacity > (size_t)-1 / 2)
            return CH_ERR_RANGE;
        capacity *= 2;
    }

    unsigned char *grown = realloc(self->data, capacity);
    if (grown == NULL)
        return CH_ERR_MEM;

    self->data     = grown;
    self->capacity = capacity;
    return CH_OK;
}

ch_err ch_buf_append(ch_buf *self, const void *restrict data, size_t size)
{
    assert(self != NULL);
    assert(data != NULL || size == 0);

    if (size == 0)
        return CH_OK;

    ch_err err = ch_buf_reserve(self, size);
    if (err != CH_OK)
        return err;

    memcpy(self->data + self->size, data, size);
    self->size += size;
    return CH_OK;
}

ch_err ch_buf_append_str(ch_buf *self, const char *restrict text)
{
    assert(text != NULL);
    return ch_buf_append(self, text, strlen(text));
}

void ch_buf_consume(ch_buf *self, size_t n)
{
    assert(self != NULL);

    if (n == 0)
        return;
    if (n >= self->size) {
        self->size = 0;
        return;
    }

    memmove(self->data, self->data + n, self->size - n);
    self->size -= n;
}

void ch_buf_clear(ch_buf *self)
{
    assert(self != NULL);
    self->size = 0;
}

size_t ch_buf_size(const ch_buf *self)
{
    assert(self != NULL);
    return self->size;
}

size_t ch_buf_capacity(const ch_buf *self)
{
    assert(self != NULL);
    return self->capacity;
}

const unsigned char *ch_buf_data(const ch_buf *self)
{
    assert(self != NULL);
    return self->data;
}

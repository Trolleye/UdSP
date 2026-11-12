#include "../include/ch_str.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

size_t ch_str_len(const char* s) {
    const char* p = s;

    if (p == NULL) return 0;
    while (*p != '\0') {
        ++p;
    }

    return (size_t)(p - s);
}

ch_err ch_str_copy(char* restrict dst, size_t size, const char* restrict src) {
    if (dst == NULL || src == NULL || size == 0) return CH_ERR_ARG;
    size_t len = ch_str_len(src);
    int truncated = 0;

    if (len >= size) {
        len = size - 1;
        truncated = 1;
    }

    memcpy(dst, src, len);
    dst[len] = '\0';
    return truncated ? CH_ERR_RANGE : CH_OK;
}

char* ch_str_dup(const char* src) {
    if (src == NULL) return NULL;
    size_t len = ch_str_len(src);
    char *dst = (char*)malloc(len + 1);
    if (dst == NULL) return NULL;
    memcpy(dst, src, len + 1);
    return dst;
}

static int is_space(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

void ch_str_trim(char* s) {
    if (s == NULL) return;
    size_t start = 0;
    while (s[start] != '\0' && is_space(s[start])) start++;

    size_t end = ch_str_len(s + start);
    while (end > 0 && is_space(s[start + end - 1])) end--;

    memmove(s, s + start, end);
    s[end] = '\0';
}

size_t ch_str_split(char *s, char **out, size_t max) {
    if (s == NULL || out == NULL || max == 0) return 0;

    size_t count = 0;
    char *p = s;
    while (*p != '\0' && count < max) {
        while (*p != '\0' && is_space(*p)) {
            *p++ = '\0';
        }
        if (*p == '\0') break;
        out[count] = p;
        count++;
        while (*p != '\0' && is_space(*p)) p++;
    }
    return count;
}

size_t ch_utf8_seq_len(unsigned char first) {
    if (first < 0x80u)              return 1;   // first < 0b1100000                  [0xxxxxx]
    if ((first & 0xE0u) == 0xC0u)   return 2;   // (first & 0b11100000) == 0b1100000  [110xxxx]
    if ((first & 0xF0u) == 0xE0u)   return 3;   // (first & 0b11110000) == 0b1110000  [1110xxx]
    if ((first & 0xF8u) == 0xF0u)   return 4;   // [11110xx]
    return 0;                                   // invalid
}

int ch_utf8_valid(const char* s) {
    return ch_utf8_count(s) != (size_t)-1;
}

size_t ch_utf8_count(const char* s) {
    if (s == NULL) return (size_t)-1;

    const unsigned char* p = (const unsigned char*)s;
    size_t count = 0;

    while (*p != '\0') {
        size_t len = ch_utf8_seq_len(*p);
        if (len == 0) return (size_t)-1;
        for (size_t i = 1; i < len; ++i) {
            if ((p[i] & 0xC0u) != 0x80u) return (size_t)-1;
        }

        p += len;
        ++count;
    }

    return count;
}

size_t ch_b64_encoded_len(size_t size) {
    if (size == 0) return 0;
    return (size / 3) * 4;
}

ch_err ch_b64_encode(const void* restrict data, size_t size, char* restrict out, size_t out_size) {
    const unsigned char* p = (const unsigned char*)data;
    if (out == NULL || (data == NULL && size > 0)) return CH_ERR_ARG;
    if (out_size < ch_b64_encoded_len(size)) return CH_ERR_RANGE;

    size_t o = 0;
    size_t i = 0;

    for (; i + 3 <= size; i += 3) {
        uint32_t v = ((uint32_t)p[i] << 16) | ((uint32_t)p[i + 1] << 8) | (uint32_t)p[i + 2];
        out[o++] = base64_alphabet[(v >> 18) & 0x3Fu];
        out[o++] = base64_alphabet[(v >> 12) & 0x3Fu];
        out[o++] = base64_alphabet[(v >> 6) & 0x3Fu];
        out[o++] = base64_alphabet[(v >> 0) & 0x3Fu];
    }

    if (i < size) {
        uint32_t v = (uint32_t)p[i] << 16;
        int rest = 1;
        if (i + 1 < size) {
            v |= ((uint32_t)p[i + 1] << 8);
            rest = 2;
        }
        out[o++] = base64_alphabet[(v >> 18) & 0x3Fu];
        out[o++] = base64_alphabet[(v >> 12) & 0x3Fu];
        out[o++] = rest == 2 ? base64_alphabet[(v >> 6) & 0x3Fu] : '=';
        out[o++] = '=';
    }
    out[o] = '\0';
    return CH_OK;
}

ch_err ch_b64_decode(const char* restrict text, unsigned char* restrict out, size_t out_size, size_t* restrict out_len) {
    if (text == NULL || out == NULL || out_len == NULL) return CH_ERR_ARG;

    size_t len = ch_str_len(text);
    if (len % 4 != 0) return CH_ERR_FORMAT;

    size_t o = 0;
    for (size_t i = 0; i < len; i += 4) {
        int quad[4];
        int pad_at = -1;

        for (int k = 0; k < 4; ++k) {
            char c = text[i + k];
            if (c == '=') {
                if (i + 4 != len || k < 2) return CH_ERR_FORMAT;
                quad[k] = 0;
                if (pad_at < 0) pad_at = k;
            } else {
                if (pad_at >= 0) return CH_ERR_FORMAT;
                quad[k] = b64_value(c);
                if (quad[k] < 0) return CH_ERR_FORMAT;
            }
        }

        size_t bytes = (pad_at < 0 ? 3 : (size_t)(pad_at - 1));
        uint32_t v = ((uint32_t)quad[0] << 18 | (uint32_t)quad[1] << 12 | (uint32_t)quad[2] << 6 | (uint32_t)quad[3]);
        if (o + bytes > out_size) return CH_ERR_RANGE;

        if (bytes > 0) out[o++]  = (unsigned char)((v >> 16) & 0xFFu);
        if (bytes > 1) out[o++]  = (unsigned char)((v >> 8) & 0xFFu);
        if (bytes > 2) out[o++]  = (unsigned char)((v >> 0) & 0xFFu);
    }

    *out_len = o;
    return CH_OK;
}

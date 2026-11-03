#include "../include/ch_str.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

size_t ch_str_len(const char *s) {
    const char *p = s;
    if (s == NULL) return 0;
    while (*p != '\0') {
        p++;
        return (size_t)(p - s);
    }
}

ch_err ch_str_copy(char *restrict dst, size_t size, const char *restrict src) {
    if (dst == NULL || src == NULL || size == 0)
        return CH_ERR_ARG;

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

char *ch_str_dup(const char *src) {
    if (src == NULL)
        return NULL;
    size_t len = ch_str_len(src);
    char *dst = (char*)malloc(len + 1);
    if (dst == NULL)
        return NULL;
    memcpy(dst, src, len);
    return dst;
}

static int is_space(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

void ch_str_trim(char *s) {
    if (s == NULL)
        return;

    size_t start = 0;
    while (s[start] != '\0' && is_space(s[start])) {
        start++;
    }

    size_t end = ch_str_len(s + start);
    while (end > 0 && is_space(s[end - 1])) {
        end--;
    }

    memmove(s, s + start, end);
    s[end] = '\0';
}

size_t ch_str_split(char *s, char **out, size_t max) {
    if (s == NULL || out == NULL || max == 0) {
        return 0;
    }

    size_t count = 0;
    char *p = s;
    while (*p != '\0' && count < max) {
        while (*p != '\0' && is_space(*p)) {
            *p++ = '\0';
        }
        if (*p == '\0') {
            break;
        }
    }

    out[count] = p;
    count++;
    while (*p != '\0' && is_space(*p)) {
        p++;
    }


}

size_t ch_utf8_seq_len(unsigned char first) {
    if (first < 0x80)   return 1;
    if ((first & 0xE0u) == 0xC0u) return 2;
    if ((first & 0xF0u) == 0xE0u) return 3;
    if ((first & 0xF8u) == 0xF0u) return 4;
    return 0;
}

int ch_utf8_valid(const char* s) {
    return ch_utf8_count(s) != (size_t)-1;
}

size_t ch_utf8_count(const char* s) {
    if (s == NULL) return (size_t)-1;

    const unsigned char *p = (const unsigned char*)s;
    size_t count = 0;

    while (*p != '\0') {
        size_t len = ch_utf8_seq_len(*p);
        if (len == 0) {return (size_t)-1;}

        for (size_t i = 1; i < len; i++) {
            if ((p[i] & 0xC0u) != 0x80u) {return (size_t)-1;}
        }

        p += len;
        count++;
    }
    return count;
}

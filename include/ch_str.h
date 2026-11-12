#ifndef CH_STR_H
#define CH_STR_H

#include "ch_err.h"
#include <stddef.h>

static const char base64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

size_t ch_str_len(const char* s); // zoberie string a povie, aký je veľký (koľko char)
ch_err ch_str_copy(char* restrict dst, size_t size, const char* restrict src); // prekopíruje string z ptr na druhý ptr
char* ch_str_dup(const char* src); // duplikuje string, vráti dyn alloc nový string
void ch_str_trim(char* s); // oreže biele znaky, atp.
size_t ch_str_split(char *s, char **out, size_t max);

size_t ch_utf8_seq_len(unsigned char first);
int ch_utf8_valid(const char* s);
size_t ch_utf8_count(const char* s);

// base64
size_t ch_b64_encoded_len(size_t size);
ch_err ch_b64_encode(const void* restrict data, size_t size, char* restrict out, size_t out_size);
ch_err ch_b64_decode(const char* restrict text, unsigned char* restrict out, size_t out_size, size_t* restrict out_len);


#endif

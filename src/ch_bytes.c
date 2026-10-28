//
// Created by Peto on 28/10/2026.
//

#include "../include/ch_bytes.h"

void ch_put_u16(unsigned char* dst, uint16_t value) {
    dst[0] = (unsigned char)((value >> 8) & 0xFFu);
    dst[1] = (unsigned char)(value & 0xFFu);
}
void ch_put_u32(unsigned char* dst, uint32_t value) {
    dst[0] = (unsigned char)((value >> 24) & 0xFFu);
    dst[1] = (unsigned char)((value >> 16) & 0xFFu);
    dst[2] = (unsigned char)((value >> 8) & 0xFFu);
    dst[3] = (unsigned char)(value & 0xFFu);
}
void ch_put_u64(unsigned char* dst, uint64_t value); // TODO

uint16_t ch_get_u16(const unsigned char* src) { return (uint16_t)(((uint16_t)src[0] << 8 | (uint16_t)src[1])); }
uint32_t ch_get_u32(const unsigned char* src) { return ((uint32_t)src[0] << 24) | ((uint32_t)src[1] << 16) | ((uint32_t)src[2] << 8) | (uint32_t)src[3]; }
uint64_t ch_get_u64(const unsigned char* src); // TODO

void ch_put_f64(unsigned char* dst, double value);
double ch_get_f64(const unsigned char* src);

int ch_is_big_endian_ptr(void);
int ch_is_big_endian_union(void);
const char* ch_endian_name(void);
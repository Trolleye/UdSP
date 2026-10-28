//
// Created by Peto on 28/10/2026.
//

#ifndef CV4_CH_BYTES_H
#define CV4_CH_BYTES_H

#include <stdint.h>

void ch_put_u16(unsigned char* dst, uint16_t value);
void ch_put_u32(unsigned char* dst, uint32_t value);
void ch_put_u64(unsigned char* dst, uint64_t value);

uint16_t ch_get_u16(const unsigned char* src);
uint32_t ch_get_u32(const unsigned char* src);
uint64_t ch_get_u64(const unsigned char* src);

void ch_put_f64(unsigned char* dst, double value);
double ch_get_f64(const unsigned char* src);

int ch_is_big_endian_ptr(void);
int ch_is_big_endian_union(void);
const char* ch_endian_name(void);

#endif //CV4_CH_BYTES_H

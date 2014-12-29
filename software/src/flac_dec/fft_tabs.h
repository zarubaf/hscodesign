#ifndef FFT_TABS_H
#define FFT_TABS_H

#include <stdint.h>

extern const int16_t cos_16[8];
extern const int16_t cos_32[16];
extern const int16_t cos_64[32];
extern const int16_t cos_128[64];
extern const int16_t cos_256[128];
extern const int16_t cos_512[256];
extern const int16_t cos_1024[512];
extern const int16_t cos_2048[1024];
extern const int16_t cos_4096[2048];
extern const uint16_t bit_rev_4096[4096];
extern const int fft_mask_4096[4096];

#endif // FFT_TABS_H

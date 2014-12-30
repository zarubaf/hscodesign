#ifndef FFT_H
#define FFT_H

#include <stdint.h>

typedef struct {
    int16_t re, im;
} fft_complex;

#define FFT_FUNC(width) void fft_##width(fft_complex *z)
#define FFT_PERM(width) void fft_##width##_permute(int16_t *in_prev, int16_t *in_post, int len, fft_complex *out)

FFT_FUNC(1024);
FFT_FUNC(2048);
FFT_FUNC(4096);

FFT_PERM(4096);

#endif // FFT_H

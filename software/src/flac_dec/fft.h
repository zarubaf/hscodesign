#ifndef FFT_H
#define FFT_H

#include <stdint.h>

typedef struct {
    int16_t re, im;
} fft_complex;

void fft_4096(fft_complex *z);
void fft_4096_permute(int16_t *in_prev, int16_t *in_post, int len, fft_complex *out);

#endif // FFT_H

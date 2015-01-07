#include "fft.h"

#include "fft_butterfly.h"
#include "fft_tabs.h"

#include <system.h>

#define SQRT1_2 23170 // (1.0 / sqrt(2)) * (1 << 15)

/*
static inline void pass(fft_complex *z, const int16_t *wre, unsigned int n)
{
    int32_t t1, t2, t3, t4, t5, t6;
    int o1 = 2*n;
    int o2 = 4*n;
    int o3 = 6*n;
    const int16_t *wim = wre+o1;
    n--;

    TRANSFORM_ZERO(z[0],z[o1],z[o2],z[o3]);
    TRANSFORM(z[1],z[o1+1],z[o2+1],z[o3+1],wre[1],wim[-1]);

    do {
        z += 2;
        wre += 2;
        wim -= 2;
        TRANSFORM(z[0],z[o1],z[o2],z[o3],wre[0],wim[0]);
        TRANSFORM(z[1],z[o1+1],z[o2+1],z[o3+1],wre[1],wim[-1]);
    } while(--n);
}
*/

static inline void fft_4(fft_complex *z)
{
    int32_t t1, t2, t3, t4, t5, t6, t7, t8;

    BF(t3, t1, z[0].re, z[1].re);
    BF(t8, t6, z[3].re, z[2].re);
    BF(z[2].re, z[0].re, t1, t6);
    BF(t4, t2, z[0].im, z[1].im);
    BF(t7, t5, z[2].im, z[3].im);
    BF(z[3].im, z[1].im, t4, t8);
    BF(z[3].re, z[1].re, t3, t7);
    BF(z[2].im, z[0].im, t2, t5);
}

static inline void fft_8(fft_complex *z)
{
    int32_t t1, t2, t3, t4, t5, t6;

    fft_4(z);

    BF(t1, z[5].re, z[4].re, -z[5].re);
    BF(t2, z[5].im, z[4].im, -z[5].im);
    BF(t5, z[7].re, z[6].re, -z[7].re);
    BF(t6, z[7].im, z[6].im, -z[7].im);

    BUTTERFLIES(z[0],z[2],z[4],z[6]);
    TRANSFORM(z[1],z[3],z[5],z[7],SQRT1_2,SQRT1_2);
}

#define DECL_FFT(n, n2, n4)         \
FFT_STATIC_INLINE void fft_##n(fft_complex *z)        \
{                                   \
    fft_##n2(z);                    \
    fft_##n4(z + n4 * 2);           \
    fft_##n4(z + n4 * 3);           \
    bf_pass_##n(z);                 \
}

#define FFT_STATIC_INLINE static inline
DECL_FFT(16,8,4)
DECL_FFT(32,16,8)
DECL_FFT(64,32,16)
DECL_FFT(128,64,32)

#undef FFT_STATIC_INLINE
#define FFT_STATIC_INLINE
DECL_FFT(256,128,64)
DECL_FFT(512,256,128)
DECL_FFT(1024,512,256)
DECL_FFT(2048,1024,512)
DECL_FFT(4096,2048,1024)

FFT_PERM(4096)
{
    fft_complex *z;
    int mask, i;
    const uint16_t *bit_rev = bit_rev_4096;

    in_prev_l += len - 2048;
    in_prev_r += len - 2048;

    mask = 0;

    for (i = 0; i < 2048; i++) {
        z = &out[*(bit_rev++)];
        //mask = *(mask_p++);
        mask += 32;
        z->re = ALT_CI_ASHIFT_R(*(in_prev_l++) * mask, 16);
        z->im = ALT_CI_ASHIFT_R(*(in_prev_r++) * mask, 16);
    }

    for (i = 0; i < 2048; i++) {
        z = &out[*(bit_rev++)];
        //mask = *(mask_p++);
        mask -= 32;
        z->re = ALT_CI_ASHIFT_R(*(in_post_l++) * mask, 16);
        z->im = ALT_CI_ASHIFT_R(*(in_post_r++) * mask, 16);
    }
}

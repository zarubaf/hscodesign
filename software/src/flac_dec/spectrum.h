#ifndef SPECTRUM_H
#define SPECTRUM_H

#include "fft.h"

#include <system.h>

#undef ALT_CI_SHIFT_LI
#define ALT_CI_SHIFT_LI(A,B) ALT_CI_SHIFT_L(A,B)

#undef ALT_CI_SHIFT_RI
#define ALT_CI_SHIFT_RI(A,B) ALT_CI_SHIFT_R(A,B)

#undef ALT_CI_ASHIFT_RI
#define ALT_CI_ASHIFT_RI(A,B) ALT_CI_ASHIFT_R(A,B)

static inline void get_freq_abs(fft_complex *freq, int idx, uint32_t *left, uint32_t *right)
{
    fft_complex zl, zr, z1, z2;

    z1 = freq[idx];
    z2 = freq[(4096 - idx) & 0xfff];
    zl.re = (z1.re + z2.re) / 2;
    zl.im = (z1.im - z2.im) / 2;
    zr.re = (z1.im + z2.im) / 2;
    zr.im = -(z1.re - z2.re) / 2;

    *left = fast_sqrt(64 * fast_sqrt((int32_t)zl.re * (int32_t)zl.re + (int32_t)zl.im * (int32_t)zl.im));
    *right = fast_sqrt(64 * fast_sqrt((int32_t)zr.re * (int32_t)zr.re + (int32_t)zr.im * (int32_t)zr.im));

    if (*left > 255)
        *left = 255;

    if (*right > 255)
        *right = 255;
}

static void get_spectrum(fft_complex *freq, uint32_t *out)
{
    uint32_t l1, l2, r1, r2, a, b, i, h;

#ifdef SPECTRO_LOG
    l1 = 0;
    r1 = 0;

    for (h = 0; h < 64; h += 4) {
        i = ALT_CI_SHIFT_RI(h, 2);
        get_freq_abs(freq, i, &l2, &r2);

        out[h] = l1 | ALT_CI_SHIFT_LI(r1, 8) | ALT_CI_SHIFT_LI((l1 + r1) & 0x1fe, 15);

        a = ALT_CI_SHIFT_R(3 * l1 + l2, 2);
        b = ALT_CI_SHIFT_R(3 * r1 + r2, 2);
        out[h + 1] = a | ALT_CI_SHIFT_LI(b, 8) | ALT_CI_SHIFT_LI((a + b) & 0x1fe, 15);

        a = ALT_CI_SHIFT_R(l1 + l2, 1);
        b = ALT_CI_SHIFT_R(r1 + r2, 1);
        out[h + 2] = a | ALT_CI_SHIFT_LI(b, 8) | ALT_CI_SHIFT_LI((a + b) & 0x1fe, 15);

        a = ALT_CI_SHIFT_R(l1 + 3 * l2, 2);
        b = ALT_CI_SHIFT_R(r1 + 3 * r2, 2);
        out[h + 3] = a | ALT_CI_SHIFT_LI(b, 8) | ALT_CI_SHIFT_LI((a + b) & 0x1fe, 15);

        l1 = l2;
        r1 = r2;
    }

    for (h = 64; h < 128; h += 2) {
        i = ALT_CI_SHIFT_RI(h, 1) - 16;
        get_freq_abs(freq, i, &l2, &r2);

        out[h] = l1 | ALT_CI_SHIFT_LI(r1, 8) | ALT_CI_SHIFT_LI((l1 + r1) & 0x1fe, 15);

        a = ALT_CI_SHIFT_RI(l1 + l2, 1);
        b = ALT_CI_SHIFT_RI(r1 + r2, 1);
        out[h + 1] = a | ALT_CI_SHIFT_LI(b, 8) | ALT_CI_SHIFT_LI((a + b) & 0x1fe, 15);

        l1 = l2;
        r1 = r2;
    }

    for (h = 128; h < 448; h++) {
        switch (ALT_CI_SHIFT_RI(h, 6)) {
            case 2:
                i = h - 80;
                break;

            case 3:
                i = ALT_CI_SHIFT_LI(h, 1) - 272;
                break;

            case 4:
                i = ALT_CI_SHIFT_LI(h, 2) - 784;
                break;

            case 5:
                i = ALT_CI_SHIFT_LI(h, 3) - 2064;
                break;

            case 6:
                i = ALT_CI_SHIFT_LI(h, 4) - 5136;
                break;
        }

        get_freq_abs(freq, i, &a, &b);
        out[h] = a | ALT_CI_SHIFT_LI(b, 8) | ALT_CI_SHIFT_LI((a + b) & 0x1fe, 15);
    }

    for (h = 448; h < 480; h++)
        out[h] = 0;
#else
    for (h = 0; h < 480; h++) {
        get_freq_abs(freq, h * 4, &a, &b);
        out[h] = a | ALT_CI_SHIFT_LI(b, 8) | ALT_CI_SHIFT_LI((a + b) & 0x1fe, 15);
    }
#endif
}

#endif // SPECTRUM_H

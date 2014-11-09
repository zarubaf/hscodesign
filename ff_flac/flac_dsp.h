#ifndef FLAC_DSP_H
#define FLAC_DSP_H

#include <stdint.h>

void flac_lpc_16_c(int32_t *decoded, const int coeffs[32], int pred_order, int qlevel, int len);

void flac_decorrelate_indep_c(int16_t **out, int32_t **in, int channels, int len, int shift);
void flac_decorrelate_ls_c(int16_t **out, int32_t **in, int channels, int len, int shift);
void flac_decorrelate_rs_c(int16_t **out, int32_t **in, int channels, int len, int shift);
void flac_decorrelate_ms_c(int16_t **out, int32_t **in, int channels, int len, int shift);

#endif // FLAC_DSP_H

#include "flac_dsp.h"

void flac_lpc_16_c(int32_t *decoded, const int coeffs[32], int pred_order, int qlevel, int len)
{
    int i, j;

    for (i = pred_order; i < len - 1; i += 2, decoded += 2) {
        int c = coeffs[0];
        int d = decoded[0];
        int s0 = 0, s1 = 0;
        for (j = 1; j < pred_order; j++) {
            s0 += c*d;
            d = decoded[j];
            s1 += c*d;
            c = coeffs[j];
        }
        s0 += c*d;
        d = decoded[j] += s0 >> qlevel;
        s1 += c*d;
        decoded[j + 1] += s1 >> qlevel;
    }
    if (i < len) {
        int sum = 0;
        for (j = 0; j < pred_order; j++)
            sum += coeffs[j] * decoded[j];
        decoded[j] += sum >> qlevel;
    }
}

void flac_decorrelate_indep_c(int16_t **out, int32_t **in, int channels, int len, int shift)
{
    int i, j;

    for (j = 0; j < len; j++)
        for (i = 0; i < channels; i++)
            out[i][j] = in[i][j] << shift;
}

void flac_decorrelate_ls_c(int16_t **out, int32_t **in, int channels, int len, int shift)
{
    int i;

    for (i = 0; i < len; i++) {
        int a = in[0][i];
        int b = in[1][i];
        out[0][i] =  a      << shift;
        out[1][i] = (a - b) << shift;
    }
}

void flac_decorrelate_rs_c(int16_t **out, int32_t **in, int channels, int len, int shift)
{
    int i;

    for (i = 0; i < len; i++) {
        int a = in[0][i];
        int b = in[1][i];
        out[0][i] = (a + b) << shift;
        out[1][i] =  b      << shift;
    }
}

void flac_decorrelate_ms_c(int16_t **out, int32_t **in, int channels, int len, int shift)
{
    int i;

    for (i = 0; i < len; i++) {
        int a = in[0][i];
        int b = in[1][i];
        a -= b >> 1;
        out[0][i] = (a + b) << shift;
        out[1][i] =  a      << shift;
    }
}


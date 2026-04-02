#include "ee_audiomark.h"
#include "ee_api.h"

static void gen_bit_rev(uint16_t *out, int n)
{
    int bits = 0;
    for (int t = n; t > 1; t >>= 1) bits++;
    for (int i = 0; i < n; i++) {
        int rev = 0, t = i;
        for (int b = 0; b < bits; b++) { rev = (rev << 1) | (t & 1); t >>= 1; }
        out[i] = (uint16_t)rev;
    }
}

ee_status_t th_rfft_init_f32(ee_rfft_f32_t *p_instance, int fft_len)
{
    if (fft_len != SRFFT_N) return EE_STATUS_ERROR;
    ee_rfft_f32_t *e = (ee_rfft_f32_t *)p_instance;

    /* --- Standard Radix-2 Twiddles (All Stages) --- */
    int offset = 0;
    for (int s = 0; s < SRFFT_STAGES; s++) {
        int step = 1 << s;
        int M    = 2 * step;

        for (int j = 0; j < step; j++) {
            float angle = -2.0f * M_PI_F * (float)j / (float)M;
            e->mt_re[offset + j] = cosf(angle);
            e->mt_im[offset + j] = sinf(angle);
        }
        offset += step;
    }

    /* --- Unpack twiddles W_N^k for real wrapper --- */
    for (int k = 0; k <= SRFFT_N2; k++) {
        float th = -2.0f * M_PI_F * (float)k / (float)SRFFT_N;
        e->up_re[k] = cosf(th);
        e->up_im[k] = sinf(th);
    }

    /* --- Bit-reversal permutation for N2 = 512 --- */
    gen_bit_rev(e->bit_rev, SRFFT_N2);

    return EE_STATUS_OK;
}

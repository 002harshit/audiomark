#include "ee_audiomark.h"
#include "ee_api.h"

#include <math.h>

static void s_cfft_core(ee_rfft_f32_t *e,
                         ee_f32_t *  xr, ee_f32_t *  xi,
                         int is_inv)
{
    const int n = SRFFT_N2;

    /* Bit-reversal */
    for (int i = 0; i < n; i++) {
        int j = e->bit_rev[i];
        if (j > i) {
            ee_f32_t tr = xr[i]; xr[i] = xr[j]; xr[j] = tr;
            ee_f32_t ti = xi[i]; xi[i] = xi[j]; xi[j] = ti;
        }
    }

    /* Standard Iterative Radix-2 DIT */
    int offset = 0;
    float sign = is_inv ? -1.0f : 1.0f;

    for (int s = 0; s < SRFFT_STAGES; s++) {
        int step = 1 << s;
        int step2 = 2 * step;

        for (int g = 0; g < n; g += step2) {
            for (int j = 0; j < step; j++) {
                int i0 = g + j;
                int i1 = i0 + step;
                
                float wr = e->mt_re[offset + j];
                float wi = e->mt_im[offset + j] * sign;

                float r0 = xr[i0], i_0 = xi[i0];
                float r1 = xr[i1], i_1 = xi[i1];

                float tr = wr * r1 - wi * i_1;
                float ti = wr * i_1 + wi * r1;

                xr[i1] = r0 - tr;
                xi[i1] = i_0 - ti;
                xr[i0] = r0 + tr;
                xi[i0] = i_0 + ti;
            }
        }
        offset += step;
    }
}

void th_rfft_f32(ee_rfft_f32_t *p_instance,
                 ee_f32_t      *p_in,
                 ee_f32_t      *p_out,
                 uint8_t        ifftFlag)
{
    BM_BEGIN();
 
    ee_rfft_f32_t *e = (ee_rfft_f32_t *)p_instance;
    ee_f32_t *xr = e->xr;
    ee_f32_t *xi = e->xi;
 
    if (ifftFlag == 0) {
        /* ----------------------------------------------------------------
         * FORWARD RFFT
         * ---------------------------------------------------------------- */
 
        for (int n = 0; n < SRFFT_N2; n++) {
            xr[n] = p_in[2 * n];
            xi[n] = p_in[2 * n + 1];
        }
 
        s_cfft_core(e, xr, xi, 0);
 
 
        /* DC (k=0) and Nyquist (k=N/2) */
        p_out[0] = xr[0] + xi[0];   /* X[0].re   */
        p_out[1] = xr[0] - xi[0];   /* X[N/2].re */
 
        /* k = 1 … N/2 - 1 */
        for (int k = 1; k < SRFFT_N2; k++) {
            float Ar = xr[k],          Ai = xi[k];
            float Br = xr[SRFFT_N2-k], Bi = xi[SRFFT_N2-k];
            float Wr = e->up_re[k],    Wi = e->up_im[k];
 
            float sum_r = Ar + Br, dif_r = Ar - Br;
            float sum_i = Ai + Bi, dif_i = Ai - Bi;
 
            p_out[2 * k]     = 0.5f * (sum_r + Wr * sum_i + Wi * dif_r);
            p_out[2 * k + 1] = 0.5f * (dif_i - Wr * dif_r + Wi * sum_i);
        }
 
    } else {
        /* ----------------------------------------------------------------
         * INVERSE RFFT
         * ----------------------------------------------------------------
         * p_in layout:  [0]=X[0].re (DC),  [1]=X[N/2].re (Nyquist),
         *               [2k]=X[k].re, [2k+1]=X[k].im  for k=1..N/2-1
         * ---------------------------------------------------------------- */
 
 
        /* DC / Nyquist special case */
        float dc  = p_in[0];   /* X[0].re  */
        float nyq = p_in[1];   /* X[N/2].re */
        xr[0] = 0.5f * (dc + nyq);
        xi[0] = 0.5f * (dc - nyq);
 
        /* k = 1 … N/2 - 1 */
        for (int k = 1; k < SRFFT_N2; k++) {
            float Ar = p_in[2 * k];
            float Ai = p_in[2 * k + 1];
            float Wr = e->up_re[k];   /* cosf(-2π k/N) */
            float Wi = e->up_im[k];   /* sinf(-2π k/N) < 0 for k>0 */
 
            /* F[k].re = Ar - Wr·Ai,  F[k].im = Wi·Ai
             * F[N/2-k]: use X[N-k]=conj(X[k]) → (Ar, -Ai):
             *   F[N/2-k].re = Ar + Wr·Ai,  F[N/2-k].im = Wi·Ai
             * (Wi is the same because up_re[N-k]=up_re[k], up_im[N-k]=-up_im[k]
             *  and we negate Ai for the conjugate, so -(-Ai)*(-Wi) = Wi*Ai) */
            xr[k] = Ar - Wr * Ai;
            xi[k] = Wi * Ai;
        }
 
        /* Fill F[N/2-k] for k=1..N/2-1 in a second pass
         * (avoids overwriting values still needed) */
        for (int k = 1; k < SRFFT_N2; k++) {
            if (SRFFT_N2 - k == k) break;   /* k = N/4, self-conjugate */
            float Ar = p_in[2 * k];
            float Ai = p_in[2 * k + 1];
            float Wr = e->up_re[k];
            float Wi = e->up_im[k];
            int   km = SRFFT_N2 - k;
 
            if (km > k) {
                /* Only fill entries km > k to avoid float-writing */
                xr[km] = Ar + Wr * Ai;
                xi[km] = Wi * Ai;
            }
        }
 
        /* Step 2: N/2-point complex IFFT.
         * IFFT = conj(input) → forward FFT → conj(result) → scale by 1/N2 */
        for (int n = 0; n < SRFFT_N2; n++) xi[n] = -xi[n];   /* conj input */
        s_cfft_core(e, xr, xi, 0 /* forward */);
        const float inv_N2 = 1.0f / (float)SRFFT_N2;
        for (int n = 0; n < SRFFT_N2; n++) {
            xr[n] *=  inv_N2;
            xi[n]  = -xi[n] * inv_N2;    /* conj + scale */
        }
 
        /* Step 3: Interleave to real output buffer */
        for (int n = 0; n < SRFFT_N2; n++) {
            p_out[2 * n]     = xr[n];
            p_out[2 * n + 1] = xi[n];
        }
    }
 
    BM_END();
    BM_PRINT();
}


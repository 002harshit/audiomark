#ifndef RISCV_RFFT_DEF_H
#define RISCV_RFFT_DEF_H

#include <math.h>


#define SRFFT_N        1024    /* Real transform length */
#define SRFFT_N2       512     /* Complex core length = N/2 */
#define SRFFT_STAGES   9       /* log2(N2) = 9 stages */

/* Total twiddles for Radix-2: 1 + 2 + 4 + ... + 256 = 511 */
#define SRFFT_TW_TOTAL 511

#ifndef M_PI_F
#define M_PI_F 3.14159265358979323846f
#endif

typedef struct {
    /* Standard Radix-2 Twiddles */
    TH_FLOAT32_TYPE mt_re[SRFFT_TW_TOTAL]; 
    TH_FLOAT32_TYPE mt_im[SRFFT_TW_TOTAL];

    /* Unpack/Post-process Twiddles for Real FFT wrapper */
    TH_FLOAT32_TYPE up_re[SRFFT_N2 + 1];
    TH_FLOAT32_TYPE up_im[SRFFT_N2 + 1];

    /* Bit-reversal table */
    uint16_t bit_rev[SRFFT_N2];

    /* Scratch buffers - Essential to align these for the core loop */
    TH_FLOAT32_TYPE xr[SRFFT_N2];
    TH_FLOAT32_TYPE xi[SRFFT_N2];
} riscv_rfft_instance_f32;

#endif // RISCV_RFFT_DEF_H

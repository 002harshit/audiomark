#include "ee_audiomark.h"
#include "ee_api.h"

#include "riscv_dsp_math_types.h"
extern int32_t riscv_dsp_rfft_f32(float32_t *src, uint32_t m);
extern int32_t riscv_dsp_rifft_f32(float32_t *src, uint32_t m);

void th_rfft_f32(ee_rfft_f32_t *p_instance,
                 ee_f32_t      *p_in,
                 ee_f32_t      *p_out,
                 uint8_t        ifftFlag)
{
    BM_BEGIN();

    const uint32_t fft_len = 1024;
    const uint32_t fft_len_log2 = 10; // log2(1024) = 10

    // libdsp processes in-place. Copy input to the output buffer.
    memcpy(p_out, p_in, fft_len * sizeof(ee_f32_t));

    if (ifftFlag == 0) {
        /* Forward Real FFT */
        // Output format: p_out[0]=DC, p_out[1]=Nyquist, followed by complex pairs
        riscv_dsp_rfft_f32((float32_t *)p_out, fft_len_log2);
    } else {
        /* Inverse Real FFT */
        riscv_dsp_rifft_f32((float32_t *)p_out, fft_len_log2);
    }

    BM_END();
    BM_PRINT();
}

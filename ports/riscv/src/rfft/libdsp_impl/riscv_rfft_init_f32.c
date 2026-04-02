#include "ee_audiomark.h"
#include "ee_api.h"

#include "riscv_dsp_math_types.h"

ee_status_t th_rfft_init_f32(ee_rfft_f32_t *p_instance, int fft_len) {
    if (fft_len != 1024) {
        return EE_STATUS_ERROR;
    }
    return EE_STATUS_OK;
}

#include <stdlib.h>

#include "riscv_audiomark.h"
#include "ee_types.h"

int riscv_rfft_init_f32(riscv_rfft_f32_t *rfft_data, int fft_len)
{
  if (fft_len <= 0 || (fft_len & (fft_len - 1)) != 0) {
    return EE_STATUS_ERROR;
  }

  rfft_data->fft_len = fft_len;

  /* Allocate internal working memory */
  rfft_data->work_real = (ee_f32_t *) malloc(fft_len * sizeof(ee_f32_t));
  rfft_data->work_imag = (ee_f32_t *) malloc(fft_len * sizeof(ee_f32_t));

  if (rfft_data->work_real == NULL || rfft_data->work_imag == NULL) {
    return EE_STATUS_ERROR;
  }

  return EE_STATUS_OK;
}

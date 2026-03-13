#include "riscv_audiomark.h"

#include "ee_types.h"

int riscv_cfft_init_f32(riscv_cfft_f32_t *cfft_data, int fft_len)
{
  if (fft_len <= 0 || (fft_len & (fft_len - 1)) != 0) {
    return EE_STATUS_ERROR;
  }
  cfft_data->fft_len = fft_len;
  return EE_STATUS_OK;
}

#include "riscv_audiomark.h"
#include <math.h>

void riscv_cmplx_mag_f32(riscv_f32_t *va_in, riscv_f32_t *vb_out, uint32_t len)
{
  for (uint32_t i = 0; i < len; i++) {
    float real = va_in[2 * i];
    float imag = va_in[2 * i + 1];
    vb_out[i] = sqrtf(real * real + imag * imag);
  }
}

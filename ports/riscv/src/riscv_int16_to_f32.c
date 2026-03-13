#include "riscv_audiomark.h"

void riscv_int16_to_f32(const int16_t *va_in, riscv_f32_t *vb_out, uint32_t len)
{
  for (uint32_t i = 0; i < len; i++) {
    vb_out[i] = (riscv_f32_t) va_in[i] / 32768.0f;
  }
}

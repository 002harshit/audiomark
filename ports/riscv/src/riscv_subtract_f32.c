#include "riscv_audiomark.h"

void riscv_subtract_f32(
  riscv_f32_t *va_in, riscv_f32_t *vb_in, riscv_f32_t *vc_out, uint32_t len
)
{
  for (uint32_t i = 0; i < len; i++) {
    vc_out[i] = va_in[i] - vb_in[i];
  }
}

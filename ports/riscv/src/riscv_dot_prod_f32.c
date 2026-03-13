#include "riscv_audiomark.h"

void riscv_dot_prod_f32(
  riscv_f32_t *va_in, riscv_f32_t *vb_in, uint32_t len, riscv_f32_t *prod_out
)
{
  riscv_f32_t accu = 0.0f;
  for (uint32_t i = 0; i < len; i++) {
    accu += va_in[i] * vb_in[i];
  }
  *prod_out = accu;
}

#include "riscv_audiomark.h"

void riscv_offset_f32(
  riscv_f32_t *va_in, riscv_f32_t offset, riscv_f32_t *vb_out, uint32_t len
)
{
  for (uint32_t i = 0; i < len; i++) {
    vb_out[i] = va_in[i] + offset;
  }
}

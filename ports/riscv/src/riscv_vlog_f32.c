#include "riscv_audiomark.h"

#include <math.h>

void riscv_vlog_f32(riscv_f32_t *va_in, riscv_f32_t *vb_out, uint32_t len)
{
  for (uint32_t i = 0; i < len; i++) {
    vb_out[i] = logf(va_in[i]);
  }
}

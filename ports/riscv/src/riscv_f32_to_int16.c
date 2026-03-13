#include "riscv_audiomark.h"

static inline int16_t saturate_to_int16(float val)
{
  if (val > 32767.0f) {
    return 32767;
  }
  if (val < -32768.0f) {
    return -32768;
  }
  return (int16_t) val;
}

void riscv_f32_to_int16(const riscv_f32_t *va_in, int16_t *vb_out, uint32_t len)
{
  for (uint32_t i = 0; i < len; i++) {
    vb_out[i] = saturate_to_int16(va_in[i] * 32768.0f);
  }
}

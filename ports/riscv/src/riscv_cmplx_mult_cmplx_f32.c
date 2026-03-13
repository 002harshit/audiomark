#include "riscv_audiomark.h"

void riscv_cmplx_mult_cmplx_f32(
  const riscv_f32_t *p_a, const riscv_f32_t *p_b, riscv_f32_t *p_c, uint32_t len
)
{
  for (uint32_t i = 0; i < len; i++) {
    riscv_f32_t realA = *p_a++;
    riscv_f32_t imagA = *p_a++;
    riscv_f32_t realB = *p_b++;
    riscv_f32_t imagB = *p_b++;

    *p_c++ = (realA * realB) - (imagA * imagB);
    *p_c++ = (realA * imagB) + (imagA * realB);
  }
}

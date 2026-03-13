#include "riscv_audiomark.h"

void riscv_cmplx_dot_prod_f32(
  const riscv_f32_t *p_a,
  const riscv_f32_t *p_b,
  uint32_t len,
  riscv_f32_t *p_r,
  riscv_f32_t *p_i
)
{
  riscv_f32_t sumR = 0.0f;
  riscv_f32_t sumI = 0.0f;

  for (uint32_t i = 0; i < len; i++) {
    riscv_f32_t aR = *p_a++;
    riscv_f32_t aI = *p_a++;
    riscv_f32_t bR = *p_b++;
    riscv_f32_t bI = *p_b++;

    sumR += (aR * bR) - (aI * bI);
    sumI += (aR * bI) + (aI * bR);
  }
  *p_r = sumR;
  *p_i = sumI;
}

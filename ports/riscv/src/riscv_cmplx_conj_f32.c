#include "riscv_audiomark.h"

void riscv_cmplx_conj_f32(
  const riscv_f32_t *p_a, riscv_f32_t *p_c, uint32_t len
)
{
  for (uint32_t i = 0; i < len; i++) {
    *p_c++ = *p_a++;
    *p_c++ = -(*p_a++);
  }
}

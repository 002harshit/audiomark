#include "riscv_audiomark.h"
#include <math.h>

void riscv_absmax_f32(
  const riscv_f32_t *p_in, uint32_t len, riscv_f32_t *p_max, uint32_t *p_index
)
{
  riscv_f32_t max_val = 0.0f;
  uint32_t max_idx = 0;

  for (uint32_t i = 0; i < len; i++) {
    riscv_f32_t curr_val = fabsf(p_in[i]);

    if (curr_val > max_val) {
      max_val = curr_val;
      max_idx = i;
    }
  }

  *p_max = max_val;
  *p_index = max_idx;
}

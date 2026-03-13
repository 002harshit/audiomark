#include "riscv_audiomark.h"

void riscv_mat_vec_mult_f32(
  riscv_matrix_f32_t m_in, riscv_f32_t *va_in, riscv_f32_t *vb_out
)
{
  const riscv_f32_t *mdata = m_in.data;
  for (uint16_t r = 0; r < m_in.rows; r++) {
    riscv_f32_t sum = 0.0f;
    for (uint16_t c = 0; c < m_in.cols; c++) {
      sum += mdata[r * m_in.cols + c] * va_in[c];
    }

    vb_out[r] = sum;
  }
}

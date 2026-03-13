#ifndef _RISCV_AUDIOMARK_H
#define _RISCV_AUDIOMARK_H

#include <stdint.h>

#if defined(__riscv) && defined(__riscv_vector) &&                             \
  (__riscv_v_intrinsic >= 1000000)
#define RISCV_VECTOR_EXT_SUPPORTED
#endif

typedef float riscv_f32_t;

typedef struct {
  int fft_len;
  riscv_f32_t *work_real;
  riscv_f32_t *work_imag;
} riscv_rfft_f32_t;

typedef struct {
  int fft_len;
} riscv_cfft_f32_t;

typedef struct {
  uint16_t rows;
  uint16_t cols;
  riscv_f32_t *data;
} riscv_matrix_f32_t;

void riscv_mat_vec_mult_f32(
  riscv_matrix_f32_t m_in, riscv_f32_t *va_in, riscv_f32_t *vb_out
);

void riscv_vlog_f32(riscv_f32_t *va_in, riscv_f32_t *vb_out, uint32_t len);

void riscv_offset_f32(
  riscv_f32_t *va_in, riscv_f32_t offset, riscv_f32_t *vb_out, uint32_t len
);

void riscv_multiply_f32(
  riscv_f32_t *va_in, riscv_f32_t *vb_in, riscv_f32_t *vc_out, uint32_t len
);

void riscv_add_f32(
  riscv_f32_t *va_in, riscv_f32_t *vb_in, riscv_f32_t *vc_out, uint32_t len
);

void riscv_subtract_f32(
  riscv_f32_t *va_in, riscv_f32_t *vb_in, riscv_f32_t *vc_out, uint32_t len
);

void riscv_dot_prod_f32(
  riscv_f32_t *va_in, riscv_f32_t *vb_in, uint32_t len, riscv_f32_t *prod_out
);

void riscv_f32_to_int16(
  const riscv_f32_t *va_in, int16_t *vb_out, uint32_t len
);

void riscv_int16_to_f32(
  const int16_t *va_in, riscv_f32_t *vb_out, uint32_t len
);

int riscv_cfft_init_f32(riscv_cfft_f32_t *cfft_data, int fft_len);

void riscv_cfft_f32(
  riscv_cfft_f32_t *cfft_data,
  riscv_f32_t *p_buf,
  uint8_t ifftFlag,
  uint8_t bitReverseFlagR
);

int riscv_rfft_init_f32(riscv_rfft_f32_t *rfft_data, int fft_len);

void riscv_rfft_f32(
  riscv_rfft_f32_t *rfft_data,
  riscv_f32_t *va_in,
  riscv_f32_t *vb_out,
  uint8_t ifftFlag
);

void riscv_cmplx_mult_cmplx_f32(
  const riscv_f32_t *p_a, const riscv_f32_t *p_b, riscv_f32_t *p_c, uint32_t len
);

void riscv_cmplx_mag_f32(riscv_f32_t *va_in, riscv_f32_t *vb_out, uint32_t len);

void riscv_cmplx_dot_prod_f32(
  const riscv_f32_t *p_a,
  const riscv_f32_t *p_b,
  uint32_t len,
  riscv_f32_t *p_r,
  riscv_f32_t *p_i
);

void riscv_cmplx_conj_f32(
  const riscv_f32_t *p_a, riscv_f32_t *p_c, uint32_t len
);

void riscv_absmax_f32(
  const riscv_f32_t *p_in, uint32_t len, riscv_f32_t *p_max, uint32_t *p_index
);

#endif // _RISCV_AUDIOMARK_H

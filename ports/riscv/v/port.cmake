include_directories(
  ${PORT_DIR}
  ${PORT_DIR}/inc
  
  # TODO: this include must be removed after removing s_ prefix in scalar port
  # as it will conflict with riscv_audiomark.h in riscv/v/inc
  ${PORT_DIR}/../inc
)

file(GLOB RISCV_V_SOURCES
  ${PORT_DIR}/th_api.c
  ${PORT_DIR}/src/*.c

  # NOTE: these sources are temporary added from scalar port as rvv port for
  # them is not yet implemented.
  ${PORT_DIR}/../src/s_riscv_cfft_init_f32.c
  ${PORT_DIR}/../src/s_riscv_cfft_f32.c
  ${PORT_DIR}/../src/s_riscv_rfft_init_f32.c
  ${PORT_DIR}/../src/s_riscv_rfft_f32.c
  ${PORT_DIR}/../src/s_riscv_absmax_f32.c
  ${PORT_DIR}/../src/s_riscv_cmplx_mult_cmplx_f32.c
  ${PORT_DIR}/../src/s_riscv_cmplx_conj_f32.c
  ${PORT_DIR}/../src/s_riscv_cmplx_dot_prod_f32.c
  ${PORT_DIR}/../src/s_riscv_dot_prod_f32.c
  ${PORT_DIR}/../src/s_riscv_cmplx_mag_f32.c
  ${PORT_DIR}/../src/s_riscv_vlog_f32.c
  ${PORT_DIR}/../src/s_riscv_mat_vec_mult_f32.c
  ${PORT_DIR}/../src/s_riscv_nn_init.c
  ${PORT_DIR}/../src/s_riscv_nn_classify.c
)

add_definitions(-DUSE_SMALLFT)

set(PORT_SOURCE 
  ${RISCV_V_SOURCES}
)

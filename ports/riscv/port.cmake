include_directories(${PORT_DIR})
include_directories(${PORT_DIR}/include)

add_definitions(-DUSE_SMALLFT)
add_compile_options("-include" "string.h")

set(PORT_SOURCE
    ${PORT_DIR}/th_api.c

    ${PORT_DIR}/src/riscv_dot_prod_f32.c
    ${PORT_DIR}/src/riscv_f32_to_int16.c
    ${PORT_DIR}/src/riscv_int16_to_f32.c
    ${PORT_DIR}/src/riscv_mat_vec_mult_f32.c
    ${PORT_DIR}/src/riscv_multiply_f32.c
    ${PORT_DIR}/src/riscv_offset_f32.c
    ${PORT_DIR}/src/riscv_subtract_f32.c
    ${PORT_DIR}/src/riscv_vlog_f32.c
    ${PORT_DIR}/src/riscv_add_f32.c
    ${PORT_DIR}/src/riscv_cfft_init_f32.c
    ${PORT_DIR}/src/riscv_cfft_f32.c
    ${PORT_DIR}/src/riscv_rfft_init_f32.c
    ${PORT_DIR}/src/riscv_rfft_f32.c
    ${PORT_DIR}/src/riscv_cmplx_mag_f32.c
    ${PORT_DIR}/src/riscv_cmplx_mult_cmplx_f32.c
    ${PORT_DIR}/src/riscv_cmplx_dot_prod_f32.c
    ${PORT_DIR}/src/riscv_cmplx_conj_f32.c
    ${PORT_DIR}/src/riscv_absmax_f32.c
)

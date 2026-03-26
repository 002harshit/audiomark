/**
 * Copyright (C) 2024 SPEC Embedded Group
 * Copyright (C) 2022 EEMBC
 * Copyright (C) 2022 Arm Limited
 *
 * All EEMBC Benchmark Software are products of EEMBC and are provided under the
 * terms of the EEMBC Benchmark License Agreements. The EEMBC Benchmark Software
 * are proprietary intellectual properties of EEMBC and its Members and is
 * protected under all applicable laws, including all applicable copyright laws.
 *
 * If you received this EEMBC Benchmark Software without having a currently
 * effective EEMBC Benchmark License Agreement, you must discontinue use.
 */

#ifndef V_RISCV_AUDIOMARK_H
#define V_RISCV_AUDIOMARK_H

#include "ee_api.h"
#include "ee_audiomark.h"

#include "../../inc/s_riscv_audiomark.h"

#if defined(__riscv) && defined(__riscv_vector) \
    && (__riscv_v_intrinsic >= 1000000)
#include <riscv_vector.h>
#else
#error "RISCV VECTOR EXTENSION v1.0 NOT SUPPORTED"
#endif

// TODO: remove these prototypes after s_ prefix is removed from scalar port
void riscv_int16_to_f32(const int16_t *p_src, ee_f32_t *p_dst, uint32_t len);

void riscv_f32_to_int16(const ee_f32_t *p_src, int16_t *p_dst, uint32_t len);

void riscv_add_f32(ee_f32_t *p_a, ee_f32_t *p_b, ee_f32_t *p_c, uint32_t len);

void riscv_subtract_f32(ee_f32_t *p_a,
                        ee_f32_t *p_b,
                        ee_f32_t *p_c,
                        uint32_t  len);

void riscv_multiply_f32(ee_f32_t *p_a,
                        ee_f32_t *p_b,
                        ee_f32_t *p_c,
                        uint32_t  len);

void riscv_offset_f32(ee_f32_t *p_a,
                      ee_f32_t  offset,
                      ee_f32_t *p_c,
                      uint32_t  len);

#endif // V_RISCV_AUDIOMARK_H

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

#include "ee_api.h"
#include "ee_audiomark.h"
#include "v_riscv_audiomark.h"

void
v_riscv_int16_to_f32(const int16_t *p_src, ee_f32_t *p_dst, uint32_t len)
{
    const float scale = 1.0f / 32768.0f;
    while (len > 0)
    {
        size_t vl = __riscv_vsetvl_e16m2(len);

        vint16m2_t v_a = __riscv_vle16_v_i16m2(p_src, vl);

        vfloat32m4_t v_result = __riscv_vfwcvt_f_x_v_f32m4(v_a, vl);
        v_result              = __riscv_vfmul_vf_f32m4(v_result, scale, vl);
        __riscv_vse32_v_f32m4(p_dst, v_result, vl);

        len -= vl;
        p_src += vl;
        p_dst += vl;
    }
}

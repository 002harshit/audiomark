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
v_riscv_f32_to_int16(const ee_f32_t *p_src, int16_t *p_dst, uint32_t len)
{
    while (len > 0)
    {
        size_t vl = __riscv_vsetvl_e32m4(len);

        vfloat32m4_t v_src = __riscv_vle32_v_f32m4(p_src, vl);
        v_src              = __riscv_vfmul_vf_f32m4(v_src, 32768.0f, vl);

        vint16m2_t v_result = __riscv_vfncvt_rtz_x_f_w_i16m2(v_src, vl);
        __riscv_vse16_v_i16m2(p_dst, v_result, vl);

        len -= vl;
        p_src += vl;
        p_dst += vl;
    }
}

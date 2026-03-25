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
v_riscv_subtract_f32(ee_f32_t *p_a, ee_f32_t *p_b, ee_f32_t *p_c, uint32_t len)
{
    while (len > 0)
    {
        size_t vl = __riscv_vsetvl_e32m4(len);

        vfloat32m4_t v_a = __riscv_vle32_v_f32m4(p_a, vl);
        vfloat32m4_t v_b = __riscv_vle32_v_f32m4(p_b, vl);

        vfloat32m4_t v_result = __riscv_vfsub_vv_f32m4(v_b, v_a, vl);
        __riscv_vse32_v_f32m4(p_c, v_result, vl);

        len -= vl;
        p_a += vl;
        p_b += vl;
        p_c += vl;
    }
}

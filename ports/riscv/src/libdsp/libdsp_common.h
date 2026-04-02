/******************************************************************************
 * Copyright (C) 2010-2025 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2025 Andes Technology Corporation. All rights reserved. *
 *                                                                            *
 * SPDX-License-Identifier: Apache-2.0                                        *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the License); you may      *
 * not use this file except in compliance with the License.                   *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 * www.apache.org/licenses/LICENSE-2.0                                        *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT    *
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.           *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 ******************************************************************************/

#ifndef RISCV_LIBDSP_COMMON_H
#define RISCV_LIBDSP_COMMON_H

#include <stdio.h>
#include <stdint.h>

/* INTERNAL NDS TYPES */

typedef int8_t              s8_t;
typedef uint8_t             u8_t;
typedef int16_t             s16_t;
typedef uint16_t            u16_t;
typedef int32_t             s32_t;
typedef uint32_t            u32_t;
typedef int64_t             s64_t;
typedef uint64_t            u64_t;
typedef float               f32_t;
typedef double              f64_t;
typedef long                long_t;
typedef unsigned long       ulong_t;
typedef long long           llong_t;
typedef unsigned long long  ullong_t;

//Signed 32-bit union type for SIMD data.
typedef union
{
    s32_t w0;
    u32_t u0;
    f32_t f0;
    long_t l0;
#if (defined(NDS32_EB) || defined(__NDS32_EB__))
    //big endian
    struct
    {
        s16_t h1;
        s16_t h0;
    } b16;
    struct
    {
        s8_t b3;
        s8_t b2;
        s8_t b1;
        s8_t b0;
    } b8;
#else
    //little endian
    struct
    {
        s16_t h0;
        s16_t h1;
    } b16;
    struct
    {
        s8_t b0;
        s8_t b1;
        s8_t b2;
        s8_t b3;
    } b8;
#endif
} union32_t;

/* DSP MATH TYPES */

#include <stdint.h>

typedef int8_t    q7_t;
typedef uint8_t   u8_t;
typedef int8_t    s8_t;
typedef int16_t   q15_t;
typedef uint16_t  u16_t;
typedef int16_t   s16_t;
typedef int32_t   q31_t;
typedef uint32_t  u32_t;
typedef int64_t   q63_t;
typedef uint64_t  u64_t;
typedef float     float32_t;
typedef double    float64_t;

#if defined (__riscv_zfh)
typedef _Float16    float16_t;
#endif

#if defined (__riscv_zfbfmin)
typedef __bf16    bf16_t;
#endif

/* INTERNAL CONFIG DEFINES */

#define DBGMSG printf

#if defined (ENA_NDS_TOOLCHAIN) && !defined (__clang__)
#define FUNC_ATTR_FFAST_MATH  __attribute__((optimize("-ffast-math")))
#else
#define FUNC_ATTR_FFAST_MATH
#endif


/* INTERNAL ISA DECLARATION */

#define ISA_FUNC_PRFX           static inline

static inline int32_t get_log2_32b(uint32_t v)
{
    uint32_t r, shift;

    r = (v > 0xFFFF) << 4;
    v >>= r;
    shift = (v > 0xFF) << 3;
    v >>= shift;
    r |= shift;
    shift = (v > 0xF) << 2;
    v >>= shift;
    r |= shift;
    shift = (v > 0x3) << 1;
    v >>= shift;
    r |= shift;
    r |= (v >> 1);

    return (int32_t)r;
}

//Get the index of Highest Bit for 32-bit value
static inline uint32_t get_highest_bit_32b(q31_t data)
{
    uint32_t idx = (uint32_t)data;
    if (idx != 0)
    {
        idx = get_log2_32b(idx) + 1;
    }
    return idx;
}

ISA_FUNC_PRFX uint32_t NDS_ISA_CLZ(q31_t data)
{
    return (uint32_t)32 - get_highest_bit_32b(data);
}

/* INTERNAL MATH_TYPES */

#ifndef RES_LOGN
#define RES_LOGN    (10)            //default is 10
#endif

//RES_N is decided by RES_LOGN and it should not be defined manully.
#ifdef RES_N
#undef RES_N
#endif
#define RES_N       (1 << RES_LOGN)

#define TWO_PI      6.28318530717959f
#define PI          3.14159265358979f
#define PI_F64      3.1415926535897932384626433832795
#define HALF_PI     1.57079632679490f

#endif

/**
 * Copyright (C) 2022 EEMBC
 *
 * All EEMBC Benchmark Software are products of EEMBC and are provided under the
 * terms of the EEMBC Benchmark License Agreements. The EEMBC Benchmark Software
 * are proprietary intellectual properties of EEMBC and its Members and is
 * protected under all applicable laws, including all applicable copyright laws.
 *
 * If you received this EEMBC Benchmark Software without having a currently
 * effective EEMBC Benchmark License Agreement, you must discontinue use.
 */

#ifndef __TH_TYPES_H
#define __TH_TYPES_H

#include "riscv_audiomark.h"
#include "smallft.h"

#define TH_FLOAT32_TYPE riscv_f32_t

#define TH_MATRIX_INSTANCE_FLOAT32_TYPE riscv_matrix_f32_t
#define TH_RFFT_INSTANCE_FLOAT32_TYPE riscv_rfft_f32_t
#define TH_CFFT_INSTANCE_FLOAT32_TYPE riscv_cfft_f32_t

#endif /* __TH_TYPES_H */

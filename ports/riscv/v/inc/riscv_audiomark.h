/**
 * Copyright 2026 Harshit Kumar Shivhare
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

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

#include "ee_audiomark.h"
#include "ee_api.h"

#include "libdsp/libdsp_common.h"

extern int32_t riscv_dsp_rfft_f32(float32_t *src, uint32_t m);
extern int32_t riscv_dsp_rifft_f32(float32_t *src, uint32_t m);

void
th_rfft_f32(ee_rfft_f32_t *p_instance,
            ee_f32_t      *p_in,
            ee_f32_t      *p_out,
            uint8_t        ifftFlag)
{
    const uint32_t fft_len      = 1024;
    const uint32_t fft_len_log2 = 10; // log2(1024) = 10

    memcpy(p_out, p_in, fft_len * sizeof(ee_f32_t));

    if (ifftFlag == 0)
    {
        riscv_dsp_rfft_f32((float32_t *)p_out, fft_len_log2);
    }
    else
    {
        riscv_dsp_rifft_f32((float32_t *)p_out, fft_len_log2);
    }
}

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

extern int32_t riscv_dsp_cfft_f32(float32_t *src, uint32_t m);
extern int32_t riscv_dsp_cifft_f32(float32_t *src, uint32_t m);

void
th_cfft_f32(ee_cfft_f32_t *p_instance,
            ee_f32_t      *p_buf,
            uint8_t        ifftFlag,
            uint8_t        bitReverseFlagR)
{
    const uint32_t fft_len      = 128;
    const uint32_t fft_len_log2 = 7; // log2(128) = 7

    if (ifftFlag == 0)
    {
        riscv_dsp_cfft_f32((float32_t *)p_buf, fft_len_log2);
    }
    else
    {
        riscv_dsp_cifft_f32((float32_t *)p_buf, fft_len_log2);
    }
}

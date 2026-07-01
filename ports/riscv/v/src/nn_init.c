/*
 * Copyright 2026 Harshit Kumar Shivhare
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ee_audiomark.h"
#include "ee_api.h"
#include "nn/functions.h"
#include "nn/support_functions.h"
#include "nn/buffer_utils.h"

#define BUF_CONV0 \
    NN_CONV_S8_BUF_SIZE(CONV_0_IN_CH, CONV_0_FILTER_W, CONV_0_FILTER_H)
#define BUF_CONV2 \
    NN_CONV_S8_BUF_SIZE(CONV_2_IN_CH, CONV_2_FILTER_W, CONV_2_FILTER_H)
#define BUF_CONV4 \
    NN_CONV_S8_BUF_SIZE(CONV_4_IN_CH, CONV_4_FILTER_W, CONV_4_FILTER_H)
#define BUF_CONV6 \
    NN_CONV_S8_BUF_SIZE(CONV_6_IN_CH, CONV_6_FILTER_W, CONV_6_FILTER_H)
#define BUF_CONV8 \
    NN_CONV_S8_BUF_SIZE(CONV_8_IN_CH, CONV_8_FILTER_W, CONV_8_FILTER_H)
#define MAX_BUF_SIZE \
    (MAX5(BUF_CONV0, BUF_CONV2, BUF_CONV4, BUF_CONV6, BUF_CONV8))

nn_context     ctx;
static uint8_t scratch_buffer[MAX_BUF_SIZE] __attribute__((aligned(8)));

extern const int8_t ds_cnn_s_layer_1_conv2d_weights[2560];
extern const int8_t ds_cnn_s_layer_3_conv2d_weights[4096];
extern const int8_t ds_cnn_s_layer_5_conv2d_weights[4096];
extern const int8_t ds_cnn_s_layer_7_conv2d_weights[4096];
extern const int8_t ds_cnn_s_layer_9_conv2d_weights[4096];

/* packed col-major [K][out_ch], written once at init by pack_weights_col_major
 */
int8_t packed_conv0[2560];
int8_t packed_conv2[4096];
int8_t packed_conv4[4096];
int8_t packed_conv6[4096];
int8_t packed_conv8[4096];

static void
pack_weights_col_major(const int8_t *src, int8_t *dst, int out_ch, int K)
{
    for (int c = 0; c < out_ch; c++)
    {
        const int8_t *src_row = src + c * K;
        int8_t       *dst_col = dst + c;
        int           k       = 0;
        size_t        vl;

        for (; k < K; k += vl)
        {
            vl          = __riscv_vsetvl_e8m8(K - k);
            vint8m8_t v = __riscv_vle8_v_i8m8(src_row + k, vl);
            __riscv_vsse8_v_i8m8(
                dst_col + (size_t)k * out_ch, (ptrdiff_t)out_ch, v, vl);
        }
    }
}

void
th_nn_init(void)
{
    ctx.size = MAX_BUF_SIZE;
    ctx.buf  = scratch_buffer;

    pack_weights_col_major(
        ds_cnn_s_layer_1_conv2d_weights, packed_conv0, 64, 40);
    pack_weights_col_major(
        ds_cnn_s_layer_3_conv2d_weights, packed_conv2, 64, 64);
    pack_weights_col_major(
        ds_cnn_s_layer_5_conv2d_weights, packed_conv4, 64, 64);
    pack_weights_col_major(
        ds_cnn_s_layer_7_conv2d_weights, packed_conv6, 64, 64);
    pack_weights_col_major(
        ds_cnn_s_layer_9_conv2d_weights, packed_conv8, 64, 64);
}

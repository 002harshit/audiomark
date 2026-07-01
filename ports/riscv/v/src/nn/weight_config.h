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

#ifndef NN_WEIGHT_OVERRIDES_H
#define NN_WEIGHT_OVERRIDES_H

extern const int8_t packed_conv0[2560];
extern const int8_t packed_conv2[4096];
extern const int8_t packed_conv4[4096];
extern const int8_t packed_conv6[4096];
extern const int8_t packed_conv8[4096];

#define NN_CONV_WEIGHTS_0 packed_conv0
#define NN_CONV_WEIGHTS_2 packed_conv2
#define NN_CONV_WEIGHTS_4 packed_conv4
#define NN_CONV_WEIGHTS_6 packed_conv6
#define NN_CONV_WEIGHTS_8 packed_conv8

#endif

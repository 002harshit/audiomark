/**
 * Copyright 2026 Sohail Raj Satapathy
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

#ifndef NN_CONVOLVE_CONFIG_H
#define NN_CONVOLVE_CONFIG_H

#define NN_KERNEL_COLS 7

/* weights packed col-major [K][out_ch] by pack_weights_col_major(), use
 * NN_FILTER_ELEM instead of filter_data[ch*num_col_a+k] */
#define NN_FILTER_ELEM(filter_data, k, ch, num_col_a, out_ch) \
    ((filter_data)[(size_t)(k) * (out_ch) + (ch)])

#endif

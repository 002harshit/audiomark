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

#include "ee_audiomark.h"
#include "ee_api.h"

ee_status_t th_rfft_init_f32(ee_rfft_f32_t *p_instance, int fft_len)
{
  if (fft_len != 1024) {
    return EE_STATUS_ERROR;
  }

  // Precompute twiddles using double for precision
  for (int i = 0; i < 1024 / 2; i++) {
    float theta = -2.0 * M_PI * i / 1024;
    p_instance->twiddles[2 * i]     = (ee_f32_t)cosf(theta);
    p_instance->twiddles[2 * i + 1] = (ee_f32_t)sinf(theta);
  }

  return EE_STATUS_OK;
}

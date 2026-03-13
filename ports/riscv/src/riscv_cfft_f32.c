#include <math.h>

#include "riscv_audiomark.h"
#include "ee_types.h"

void riscv_cfft_f32(
  riscv_cfft_f32_t *cfft_data,
  riscv_f32_t *va,
  uint8_t ifftFlag,
  uint8_t bitReverseFlagR
)
{
  int n = cfft_data->fft_len;

  if (bitReverseFlagR) {
    int j = 0;
    for (int i = 0; i < n - 1; i++) {
      if (i < j) {
        /* Swap real parts */
        ee_f32_t temp_r = va[2 * i];
        va[2 * i] = va[2 * j];
        va[2 * j] = temp_r;

        /* Swap imaginary parts */
        ee_f32_t temp_i = va[2 * i + 1];
        va[2 * i + 1] = va[2 * j + 1];
        va[2 * j + 1] = temp_i;
      }
      int k = n / 2;
      while (k <= j) {
        j -= k;
        k /= 2;
      }
      j += k;
    }
  }

  /* 2. Cooley-Tukey Radix-2 Butterfly Stage */
  for (int step = 1; step < n; step *= 2) {
    /* Determine the angle based on forward/inverse direction */
    float theta = (ifftFlag ? 1.0f : -1.0f) * M_PI / step;

    /* Pre-calculate step twiddle factors */
    float w_step_r = cosf(theta);
    float w_step_i = sinf(theta);

    for (int i = 0; i < n; i += 2 * step) {
      float w_r = 1.0f;
      float w_i = 0.0f;

      for (int j = 0; j < step; j++) {
        int idx1 = 2 * (i + j);
        int idx2 = 2 * (i + j + step);

        /* Complex Multiplication for the Butterfly:
         * t = W * va[idx2]
         */
        float t_r = (w_r * va[idx2]) - (w_i * va[idx2 + 1]);
        float t_i = (w_r * va[idx2 + 1]) + (w_i * va[idx2]);

        /* Update the elements in place */
        va[idx2] = va[idx1] - t_r;
        va[idx2 + 1] = va[idx1 + 1] - t_i;

        va[idx1] += t_r;
        va[idx1 + 1] += t_i;

        /* Rotate twiddle factor for the next iteration */
        float next_w_r = (w_r * w_step_r) - (w_i * w_step_i);
        float next_w_i = (w_r * w_step_i) + (w_i * w_step_r);
        w_r = next_w_r;
        w_i = next_w_i;
      }
    }
  }

  /* 3. Scaling Stage (Inverse FFT Only) */
  /* The textbook inverse FFT calculation requires scaling by 1/N. */
  if (ifftFlag) {
    float inv_n = 1.0f / n;
    for (int i = 0; i < 2 * n; i++) {
      va[i] *= inv_n;
    }
  }
}

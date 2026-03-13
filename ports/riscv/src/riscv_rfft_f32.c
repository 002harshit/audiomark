#include "riscv_audiomark.h"
#include "ee_types.h"
#include <math.h>

static void radix2_bit_reverse(riscv_f32_t *real, riscv_f32_t *imag, int n)
{
  int j = 0;
  for (int i = 0; i < n - 1; i++) {
    if (i < j) {
      riscv_f32_t temp_r = real[i];
      riscv_f32_t temp_i = imag[i];
      real[i] = real[j];
      imag[i] = imag[j];
      real[j] = temp_r;
      imag[j] = temp_i;
    }
    int k = n / 2;
    while (k <= j) {
      j -= k;
      k /= 2;
    }
    j += k;
  }
}

static void
radix2_cfft(riscv_f32_t *real, riscv_f32_t *imag, int n, uint8_t is_inverse)
{
  radix2_bit_reverse(real, imag, n);

  for (int step = 1; step < n; step *= 2) {
    float theta = (is_inverse ? 1.0f : -1.0f) * M_PI / step;
    float w_step_r = cosf(theta);
    float w_step_i = sinf(theta);

    for (int i = 0; i < n; i += 2 * step) {
      float w_r = 1.0f;
      float w_i = 0.0f;
      for (int j = 0; j < step; j++) {
        // Butterfly calculation
        float t_r = w_r * real[i + j + step] - w_i * imag[i + j + step];
        float t_i = w_r * imag[i + j + step] + w_i * real[i + j + step];

        real[i + j + step] = real[i + j] - t_r;
        imag[i + j + step] = imag[i + j] - t_i;

        real[i + j] += t_r;
        imag[i + j] += t_i;

        // Rotate twiddle factor
        float next_w_r = w_r * w_step_r - w_i * w_step_i;
        float next_w_i = w_r * w_step_i + w_i * w_step_r;
        w_r = next_w_r;
        w_i = next_w_i;
      }
    }
  }

  // Scale for inverse transform
  if (is_inverse) {
    for (int i = 0; i < n; i++) {
      real[i] /= n;
      imag[i] /= n;
    }
  }
}

void riscv_rfft_f32(
  riscv_rfft_f32_t *rfft_data,
  riscv_f32_t *va_in,
  riscv_f32_t *vb_out,
  uint8_t ifftFlag
)
{
  int n = rfft_data->fft_len;
  riscv_f32_t *real = rfft_data->work_real;
  riscv_f32_t *imag = rfft_data->work_imag;

  if (ifftFlag == 0) {
    /* --- FORWARD RFFT --- */
    // 1. Load pure real input
    for (int i = 0; i < n; i++) {
      real[i] = va_in[i];
      imag[i] = 0.0f;
    }

    // 2. Compute Complex FFT
    radix2_cfft(real, imag, n, 0);

    // 3. Pack output (CMSIS-DSP standard packing)
    vb_out[0] = real[0];     // DC bin (real)
    vb_out[1] = real[n / 2]; // Nyquist bin (real)

    for (int i = 1; i < n / 2; i++) {
      vb_out[2 * i] = real[i];
      vb_out[2 * i + 1] = imag[i];
    }
  } else {
    /* --- INVERSE RFFT --- */
    // 1. Unpack symmetric complex data
    real[0] = va_in[0];
    imag[0] = 0.0f;

    real[n / 2] = va_in[1];
    imag[n / 2] = 0.0f;

    for (int i = 1; i < n / 2; i++) {
      real[i] = va_in[2 * i];
      imag[i] = va_in[2 * i + 1];

      // Reconstruct the symmetric conjugate half
      real[n - i] = va_in[2 * i];
      imag[n - i] = -va_in[2 * i + 1];
    }

    // 2. Compute Complex IFFT
    radix2_cfft(real, imag, n, 1);

    // 3. Extract the purely real output
    for (int i = 0; i < n; i++) {
      vb_out[i] = real[i];
    }
  }
}

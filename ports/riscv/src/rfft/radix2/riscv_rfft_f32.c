#include "ee_audiomark.h"
#include "ee_api.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif


#include <math.h>
#include <string.h>

static void bit_reverse(ee_f32_t *real, ee_f32_t *imag, int n) {
  int j = 0;
  for (int i = 0; i < n; i++) {
    if (i < j) {
      float tr = real[i]; real[i] = real[j]; real[j] = tr;
      float ti = imag[i]; imag[i] = imag[j]; imag[j] = ti;
    }
    int m = n >> 1;
    while (m >= 1 && j >= m) { j -= m; m >>= 1; }
    j += m;
  }
}

static void dif_cfft(ee_f32_t *real, ee_f32_t *imag, int n, const ee_f32_t *twiddles, uint8_t is_inverse) {
  for (int len = n; len >= 2; len >>= 1) {
    int half = len >> 1;
    int step = n / len;
    for (int i = 0; i < n; i += len) {
      for (int j = 0; j < half; j++) {
        int i_low = i + j;
        int i_high = i + j + half;

        float tr = real[i_low] - real[i_high];
        float ti = imag[i_low] - imag[i_high];

        real[i_low] += real[i_high];
        imag[i_low] += imag[i_high];

        float wr = twiddles[2 * j * step];
        float wi = is_inverse ? -twiddles[2 * j * step + 1] : twiddles[2 * j * step + 1];

        real[i_high] = tr * wr - ti * wi;
        imag[i_high] = tr * wi + ti * wr;
      }
    }
  }
  bit_reverse(real, imag, n);

  if (is_inverse) {
    float scale = 1.0f / n;
    for (int i = 0; i < n; i++) { real[i] *= scale; imag[i] *= scale; }
  }
}

void th_rfft_f32(ee_rfft_f32_t *p_instance,
    ee_f32_t      *p_in,
    ee_f32_t      *p_out,
    uint8_t        ifftFlag)
{
  BM_BEGIN();
  const int n = 1024;
  ee_f32_t *wr = p_instance->work_real;
  ee_f32_t *wi = p_instance->work_imag;

  if (ifftFlag == 0) {
    for (int i = 0; i < n; i++) { wr[i] = p_in[i]; wi[i] = 0.0f; }

    dif_cfft(wr, wi, n, p_instance->twiddles, 0);

    p_out[0] = wr[0];       // DC (Real)
    p_out[1] = wr[n / 2];   // Nyquist (Real)
    for (int i = 1; i < n / 2; i++) {
      p_out[2 * i]     = wr[i];
      p_out[2 * i + 1] = wi[i];
    }
  } else {
    wr[0] = p_in[0]; wi[0] = 0.0f;
    wr[n / 2] = p_in[1]; wi[n / 2] = 0.0f;
    for (int i = 1; i < n / 2; i++) {
      wr[i] = p_in[2 * i]; 
      wi[i] = p_in[2 * i + 1];
      wr[n - i] = p_in[2 * i]; 
      wi[n - i] = -p_in[2 * i + 1];
    }

    dif_cfft(wr, wi, n, p_instance->twiddles, 1);
    memcpy(p_out, wr, n * sizeof(ee_f32_t));
  }
  BM_END();
  BM_PRINT();
}

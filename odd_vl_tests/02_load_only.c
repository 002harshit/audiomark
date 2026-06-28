#include <riscv_vector.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_BINS 255

int
main(void)
{
    float *X_buf = malloc((NUM_BINS * 2 + 1) * sizeof(float));

    for (int i = 0; i < NUM_BINS * 2 + 1; i++)
    {
        X_buf[i] = (float)(i + 1) * 0.001f;
    }

    float *pX_cplx  = X_buf + 1;
    int    num_bins = NUM_BINS;

    while (num_bins > 0)
    {
        size_t vl = __riscv_vsetvl_e32m4(num_bins);

        vfloat32m4x2_t seg_x = __riscv_vlseg2e32_v_f32m4x2(pX_cplx, vl);
        vfloat32m4_t   v_xr  = __riscv_vget_v_f32m4x2_f32m4(seg_x, 0);
        vfloat32m4_t   v_xi  = __riscv_vget_v_f32m4x2_f32m4(seg_x, 1);
        (void)v_xr;
        (void)v_xi;

        pX_cplx += 2 * vl;
        num_bins -= (int)vl;
    }

    free(X_buf);
    printf("no segfault\n");
    return 0;
}
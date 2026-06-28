#include <assert.h>
#include <riscv_vector.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_BINS 255

int
main(void)
{
    float *X_buf   = malloc(NUM_BINS * 2 * sizeof(float));
    float *Y_buf   = malloc(NUM_BINS * 2 * sizeof(float));
    float *acc_buf = malloc(NUM_BINS * 2 * sizeof(float));

    // assert for 16 byte alignment
    assert(((unsigned long)X_buf % 16) == 0);
    assert(((unsigned long)Y_buf % 16) == 0);
    assert(((unsigned long)acc_buf % 16) == 0);

    for (int i = 0; i < NUM_BINS * 2; i++)
    {
        X_buf[i]   = (float)(i + 1) * 0.001f;
        Y_buf[i]   = (float)(i + 1) * 0.002f;
        acc_buf[i] = 0.0f;
    }

    float *pX_cplx   = X_buf;
    float *pY_cplx   = Y_buf;
    float *pAcc_cplx = acc_buf;

    int num_bins = NUM_BINS;

    while (num_bins > 0)
    {
        size_t vl = __riscv_vsetvl_e32m4(num_bins);

        vfloat32m4x2_t seg_x = __riscv_vlseg2e32_v_f32m4x2(pX_cplx, vl);
        vfloat32m4_t   v_xr  = __riscv_vget_v_f32m4x2_f32m4(seg_x, 0);
        vfloat32m4_t   v_xi  = __riscv_vget_v_f32m4x2_f32m4(seg_x, 1);

        vfloat32m4x2_t seg_y = __riscv_vlseg2e32_v_f32m4x2(pY_cplx, vl);
        vfloat32m4_t   v_yr  = __riscv_vget_v_f32m4x2_f32m4(seg_y, 0);
        vfloat32m4_t   v_yi  = __riscv_vget_v_f32m4x2_f32m4(seg_y, 1);

        vfloat32m4x2_t seg_acc = __riscv_vlseg2e32_v_f32m4x2(pAcc_cplx, vl);
        vfloat32m4_t   v_accr  = __riscv_vget_v_f32m4x2_f32m4(seg_acc, 0);
        vfloat32m4_t   v_acci  = __riscv_vget_v_f32m4x2_f32m4(seg_acc, 1);

        v_accr = __riscv_vfmacc_vv_f32m4(v_accr, v_xr, v_yr, vl);
        v_accr = __riscv_vfnmsac_vv_f32m4(v_accr, v_xi, v_yi, vl);

        v_acci = __riscv_vfmacc_vv_f32m4(v_acci, v_xi, v_yr, vl);
        v_acci = __riscv_vfmacc_vv_f32m4(v_acci, v_xr, v_yi, vl);

        seg_acc = __riscv_vset_v_f32m4_f32m4x2(seg_acc, 0, v_accr);
        seg_acc = __riscv_vset_v_f32m4_f32m4x2(seg_acc, 1, v_acci);
        __riscv_vsseg2e32_v_f32m4x2(pAcc_cplx, seg_acc, vl);

        pX_cplx += 2 * vl;
        pY_cplx += 2 * vl;
        pAcc_cplx += 2 * vl;
        num_bins -= (int)vl;
    }

    free(X_buf);
    free(Y_buf);
    free(acc_buf);

    printf("no segfault\n");
    return 0;
}
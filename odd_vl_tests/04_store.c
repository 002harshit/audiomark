#include <riscv_vector.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_BINS 255

int
main(void)
{
    float *acc_buf = malloc((NUM_BINS * 2 + 1) * sizeof(float));

    for (int i = 0; i < NUM_BINS * 2 + 1; i++)
    {
        acc_buf[i] = 0.0f;
    }

    float *pAcc_cplx = acc_buf + 1;
    int    num_bins  = NUM_BINS;

    while (num_bins > 0)
    {
        size_t vl = __riscv_vsetvl_e32m4(num_bins);

        vfloat32m4_t v_zero = __riscv_vfmv_v_f_f32m4(0.0f, vl);

        vfloat32m4x2_t seg_acc = __riscv_vundefined_f32m4x2();
        seg_acc = __riscv_vset_v_f32m4_f32m4x2(seg_acc, 0, v_zero);
        seg_acc = __riscv_vset_v_f32m4_f32m4x2(seg_acc, 1, v_zero);

        __riscv_vsseg2e32_v_f32m4x2(pAcc_cplx, seg_acc, vl);

        pAcc_cplx += 2 * vl;
        num_bins -= (int)vl;
    }

    free(acc_buf);
    printf("no segfault\n");
    return 0;
}
#include <riscv_vector.h>
#include <stdio.h>
#include <stdlib.h>

#define N 255

int
main(void)
{
    float *buf = malloc((N * 2 + 1) * sizeof(float));

    for (int i = 0; i < N * 2 + 1; i++)
    {
        buf[i] = (float)i;
    }

    float *p   = buf + 1;
    int    rem = N;

    while (rem > 0)
    {
        size_t vl = __riscv_vsetvl_e32m4(rem);

        vfloat32m4x2_t seg = __riscv_vlseg2e32_v_f32m4x2(p, vl);
        vfloat32m4_t   re  = __riscv_vget_v_f32m4x2_f32m4(seg, 0);
        (void)re;

        p += 2 * vl;
        rem -= (int)vl;
    }

    free(buf);
    printf("no segfault\n");
    return 0;
}
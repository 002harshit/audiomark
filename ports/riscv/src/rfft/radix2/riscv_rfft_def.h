#ifndef RISCV_RFFT_DEF_H
#define RISCV_RFFT_DEF_H

typedef struct {
    TH_FLOAT32_TYPE  work_real[1024];
    TH_FLOAT32_TYPE  work_imag[1024];
    TH_FLOAT32_TYPE  twiddles[1024];
} riscv_rfft_instance_f32;

#endif // RISCV_RFFT_DEF_H

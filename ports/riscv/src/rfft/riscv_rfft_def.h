#ifndef RISCV_RFFT_DEF_H
#define RISCV_RFFT_DEF_H

typedef struct
{
    int              fft_len;
    TH_FLOAT32_TYPE *work_real;
    TH_FLOAT32_TYPE *work_imag;
} riscv_rfft_instance_f32;


#endif // RISCV_RFFT_DEF_H

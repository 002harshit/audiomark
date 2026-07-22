# RISC-V Vector (RVV 1.0) Support

## Overview

This directory contains an implementation of selected DSP and NN kernels targeting
the RISC-V Vector Extension (RVV) version 1.0 using the standard RVV C intrinsics.

The implementation follows the ratified RVV 1.0 specification and is designed to
operate across the standard vector extension as well as embedded Zve profiles.

Reference intrinsics specification:

https://docs.riscv.org/reference/vector-c-intrinsics/_attachments/v-intrinsic-spec.pdf

---

## Toolchain & Dependencies

This implementation requires compiler support for the RVV 1.0 intrinsics API.

### GCC

- GCC 14 or newer.

  GCC 13 and earlier do not provide the vector tuple type support required by the
  RVV 1.0 intrinsics interface.

### LLVM/Clang

- Clang 19 or newer.

---

## Supported Architectures

This implementation has been tested with the full `V` extension, `Zve32x`, `Zve32f`, `Zve64x` and `Zve64f`.

The implementation is vector-length agnostic (VLA) and relies only on the standard
RVV 1.0 programming model.

---

## Vector Floating-Point Support

Some kernels make use of RVV single-precision floating-point instructions.

During CMake configuration, compiler support for vector floating-point intrinsics is
detected automatically.

If vector floating-point support is unavailable (for example when targeting
`Zve32x`), those kernels automatically fall back to their scalar floating-point
implementations while all integer vectorized kernels continue to use RVV.

When vector floating-point support is available (for example on `Zve32f` or
targets with the full `V` extension), the RVV floating-point implementations are
selected.

No additional CMake options or compiler flags are required to enable this
selection.

## Known Compiler Bug: GCC vsetvli Miscompilation in mdf_opt_rvv.c
 
While implementing `mdf_opt_rvv.c`, a GCC code generation bug was hit that caused
a segfault. The bug was traced to a dead `vsetvli` instruction being scheduled
into the `a0` register immediately before a call to `memset`, clobbering the
destination pointer argument that was expected to still be live in `a0`. It
manifested when RVV intrinsic code was placed adjacent to a scalar
zero-initialization loop that GCC compiles it into `memset` calls due to loop-distribute-patterns.
 
- **Compiler version that caused the bug:** GCC 15.2.0 (CFLAGS `-O3
  -march=rv64gcv_zba_zbb_zbc_zbs_zvl256b -mrvv-vector-bits=zvl`)
- **Source of the bug:** `speex_echo_cancellation` in `lib/speexdsp/libspeexdsp/mdf.c`
- **Commit to reproduce the bug:** [b044b6f](https://github.com/sohail103/audiomark/commit/b044b6f13a8ca205bf63a5f99cf09a79f625c0cd)
- **Bug report:** [GCC PR 126244](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=126244)
- **Original bug leading to the fix:** [GCC PR 122448](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=122448)

The fix was backported to GCC 15.3. Toolchains at or above this version should
not exhibit the issue.

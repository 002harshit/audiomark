include_directories(
    ${PORT_DIR}/..
    ${PORT_DIR}/../src
)

add_definitions(-DUSE_RISCV_DSP)

set(PORT_SOURCE 

    ${PORT_DIR}/../th_api.c

    ${PORT_DIR}/../src/dsp/tables.c
    ${PORT_DIR}/../src/dsp/cfft_f32.c
    ${PORT_DIR}/../src/dsp/rfft_fast_f32.c

    ${PORT_DIR}/../src/nn/avgpool_s8.c
    ${PORT_DIR}/../src/nn/convolve_s8.c
    ${PORT_DIR}/../src/nn/depthwise_conv_s8.c
    ${PORT_DIR}/../src/nn/fully_connected_s8.c
    ${PORT_DIR}/../src/nn/mat_mult_kernel_s8_s16.c
    ${PORT_DIR}/../src/nn/q7_to_q15_with_offset.c
    ${PORT_DIR}/../src/nn/softmax_s8.c
    ${PORT_DIR}/../src/nn/vec_mat_mult_t_s8.c
    
    ${PORT_DIR}/../src/absmax_f32.c
    ${PORT_DIR}/src/add_f32.c
    ${PORT_DIR}/../src/cfft_f32.c
    ${PORT_DIR}/../src/cfft_init_f32.c
    ${PORT_DIR}/../src/cmplx_conj_f32.c
    ${PORT_DIR}/../src/cmplx_dot_prod_f32.c
    ${PORT_DIR}/../src/cmplx_mag_f32.c
    ${PORT_DIR}/../src/cmplx_mult_cmplx_f32.c
    ${PORT_DIR}/../src/dot_prod_f32.c
    ${PORT_DIR}/src/f32_to_int16.c
    ${PORT_DIR}/src/int16_to_f32.c
    ${PORT_DIR}/../src/mat_vec_mult_f32.c
    ${PORT_DIR}/src/multiply_f32.c
    ${PORT_DIR}/../src/nn_classify.c
    ${PORT_DIR}/../src/nn_init.c
    ${PORT_DIR}/src/offset_f32.c
    ${PORT_DIR}/../src/rfft_f32.c
    ${PORT_DIR}/../src/rfft_init_f32.c
    ${PORT_DIR}/src/subtract_f32.c
    ${PORT_DIR}/../src/vlog_f32.c
)

# ============================================================
# RVV Rollback: Three-phase compilation pipeline
#
# Activated when THEAD_COMPILER is defined. Requires:
#   -DTHREAD_COMPILER=<path-to-thead-clang-or-gcc>
#   -DRVV_ROLLBACK_PY=<path-to-rvv-rollback.py>
#   -DTHEAD_MARCH=<thead-arch-string>   (default: rv64gcv0p7)
#
# Pipeline per source file:
#   [1] v1.0 clang  : .c  → .o.v1p0.s   (RVV v1.0 assembly)
#   [2] rvv-rollback: .o.v1p0.s → .o.v0p7.s  (RVV v0.7 assembly)
#   [3] thead clang : .o.v0p7.s → .o    (native T-Head object)
#   [L] thead clang : .o … → binary     (static link)
# ============================================================
if(DEFINED THEAD_COMPILER)

    if(NOT DEFINED RVV_ROLLBACK_PY)
        message(FATAL_ERROR
            "RVV rollback mode requires -DRVV_ROLLBACK_PY=<path/to/rvv-rollback.py>")
    endif()

    # Default T-Head march string; override with -DTHEAD_MARCH=...
    if(NOT DEFINED THEAD_MARCH)
        set(THEAD_MARCH "rv64gcv0p7")
    endif()

    # --------------------------------------------------------
    # Build two derived flag sets from the initial CMAKE_C_FLAGS:
    #   BASE_FLAGS_NO_MARCH  – everything except the -march=... token.
    #                          Used for the thead assemble and link steps
    #                          so we can substitute the thead march.
    # --------------------------------------------------------
    string(REGEX REPLACE "-march=[^ ]+" "" _base_no_march "${CMAKE_C_FLAGS}")
    string(STRIP "${_base_no_march}" BASE_FLAGS_NO_MARCH)

    string(REGEX REPLACE "-march=[^ ]+" "" _link_no_march "${CMAKE_EXE_LINKER_FLAGS}")
    string(STRIP "${_link_no_march}" LINK_FLAGS_NO_MARCH)

    # --------------------------------------------------------
    # Step 1 uses CMAKE_C_COMPILER (v1.0 LLVM clang) with the
    #         full <FLAGS> (including -march=rv64gcbv).
    # Step 2 runs rvv-rollback.py on the v1.0 assembly.
    # Step 3 uses THEAD_COMPILER with the thead march instead.
    #
    # <DEFINES> and <INCLUDES> are only passed to step 1 (they
    # are irrelevant for steps 2 and 3 which work on .s files).
    # --------------------------------------------------------
    set(CMAKE_C_COMPILE_OBJECT
        # [1] v1.0 clang: compile C → RVV-v1.0 assembly
        "<CMAKE_C_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -S -o <OBJECT>.v1p0.s -c <SOURCE>"

        # [2] rvv-rollback.py: translate v1.0 → v0.7 assembly
        "python3 ${RVV_ROLLBACK_PY} <OBJECT>.v1p0.s -o <OBJECT>.v0p7.s"

        # [3] thead clang: assemble v0.7 assembly → object file
        #     Use thead march; omit v1.0 march; optimisation and -static are kept.
        "${THEAD_COMPILER} -march=${THEAD_MARCH} ${BASE_FLAGS_NO_MARCH} -c -o <OBJECT> <OBJECT>.v0p7.s"
    )

    # --------------------------------------------------------
    # Link: all objects are already in v0.7 format, so use
    #       the thead compiler/linker directly.
    #       Drop v1.0 march from both compiler and linker flags.
    # --------------------------------------------------------
    set(CMAKE_C_LINK_EXECUTABLE
        "${THEAD_COMPILER} -march=${THEAD_MARCH} ${BASE_FLAGS_NO_MARCH} ${LINK_FLAGS_NO_MARCH} <OBJECTS> -o <TARGET> <LINK_LIBRARIES>"
    )

    message(STATUS "=== RVV Rollback pipeline enabled ===")
    message(STATUS "  [1] ${CMAKE_C_COMPILER}")
    message(STATUS "      flags : ${CMAKE_C_FLAGS}  (march=rv64gcbv for v1.0 intrinsics)")
    message(STATUS "      output: <obj>.v1p0.s")
    message(STATUS "  [2] python3 ${RVV_ROLLBACK_PY}")
    message(STATUS "      output: <obj>.v0p7.s")
    message(STATUS "  [3] ${THEAD_COMPILER}")
    message(STATUS "      flags : -march=${THEAD_MARCH} ${BASE_FLAGS_NO_MARCH}")
    message(STATUS "      output: <obj>.o")
    message(STATUS "  [L] ${THEAD_COMPILER} -march=${THEAD_MARCH} (linker)")
    message(STATUS "======================================")

else()
    message(STATUS "THEAD_COMPILER not set — building natively with CMAKE_C_COMPILER only.")
    message(STATUS "  To enable the RVV rollback pipeline add:")
    message(STATUS "    -DTHEAD_COMPILER=<path>  -DRVV_ROLLBACK_PY=<path>")
endif()

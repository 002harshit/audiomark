include_directories(${PORT_DIR})

file(GLOB PORT_SOURCES
    ${PORT_DIR}/*.c
    ${PORT_DIR}/scalar/*.c
)

if(NOT DISABLE_RISCV_P_EXT)
    file(GLOB SIMD_SOURCES ${PORT_DIR}/packed_simd/*.c)
else()
    list(APPEND PORT_SOURCES ${SIMD_SOURCES})
    add_definitions(-DDISABLE_RISCV_P_EXT)
endif()

if(NOT DISABLE_RISCV_V_EXT)
    file(GLOB VECTOR_SOURCES ${PORT_DIR}/vector/*.c)
    list(APPEND PORT_SOURCES ${VECTOR_SOURCES})
else()
    add_definitions(-DDISABLE_RISCV_V_EXT)
endif()

add_definitions(-DUSE_SMALLFT)

set(PORT_SOURCE ${PORT_SOURCES})

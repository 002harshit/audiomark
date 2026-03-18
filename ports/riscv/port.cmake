include_directories(${PORT_DIR})

file(GLOB PORT_SOURCES
    ${PORT_DIR}/*.c
    ${PORT_DIR}/scalar/*.c
    ${PORT_DIR}/vector/*.c
    ${PORT_DIR}/packed_simd/*.c
)

add_definitions(-DUSE_SMALLFT)

set(PORT_SOURCE ${PORT_SOURCES})

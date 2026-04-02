include_directories(
  ${PORT_DIR}
  ${PORT_DIR}/libdsp/include
  ${PORT_DIR}/libdsp/internal
  ${PORT_DIR}/src/rfft
)

file(GLOB PORT_SOURCES
  ${PORT_DIR}/libdsp/transform/*.c
  ${PORT_DIR}/libdsp/utils/table/*.c
  ${PORT_DIR}/th_api.c
  ${PORT_DIR}/src/*.c
  ${PORT_DIR}/src/rfft/*.c
)

# TODO: Add our FFT functions as an FFT backend in fftwrap.c
# for now, we enable but do not use smallfft since fftwrap.c
# throws an error without it.
add_definitions(-DUSE_SMALLFT)

set(PORT_SOURCE 
    ${PORT_SOURCES}
)

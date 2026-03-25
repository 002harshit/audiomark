include_directories(
  ${PORT_DIR}
  ${PORT_DIR}/inc
)

file(GLOB RISCV_V_SOURCES
  ${PORT_DIR}/th_api.c
  ${PORT_DIR}/src/*.c
)

add_definitions(-DUSE_SMALLFT)

set(PORT_SOURCE 
  ${RISCV_V_SOURCES}
)

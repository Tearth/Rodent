cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW)
project(kernel LANGUAGES C ASM)

add_executable(kernel.elf
    $<TARGET_OBJECTS:drv>
    src/main.c
    src/log.c
    src/arch/riscv/startup.s
)

target_link_options(kernel.elf PRIVATE
    -T ${CMAKE_CURRENT_SOURCE_DIR}/ld/$ENV{TARGET_CPU}_$ENV{TARGET_ARCH}.ld -nostartfiles
    -Wl,-Map=${CMAKE_BINARY_DIR}/kernel.map
)

if($ENV{TARGET_CPU} STREQUAL "rp2350")
    target_sources(kernel.elf PRIVATE src/hal/hal_rp2350.c)
endif()
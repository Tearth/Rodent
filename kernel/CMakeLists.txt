cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW)
project(kernel LANGUAGES C ASM)

add_executable(kernel.elf
    $<TARGET_OBJECTS:mcu>
    src/main.c
    src/log.c
    src/arch/riscv/startup.s
    src/arch/riscv/irq.s
    src/arch/riscv/irq.c
    src/mcu/mcu_rp2350.c
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_options(kernel.elf PRIVATE
    -T ${CMAKE_CURRENT_SOURCE_DIR}/ld/$ENV{TARGET_MCU}_$ENV{TARGET_ARCH}.ld -nostartfiles
    -Wl,-Map=${CMAKE_BINARY_DIR}/kernel.map
)
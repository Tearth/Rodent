cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW)
project(kernel LANGUAGES C ASM)

add_executable(kernel.elf
    $<TARGET_OBJECTS:mcu>
    src/main.c
    src/log.c
    src/sched.c
)

if($ENV{TARGET_MCU} STREQUAL "rp2350")
    target_sources(kernel.elf PRIVATE src/mcu/mcu_rp2350.c)
endif()

if($ENV{TARGET_ARCH} STREQUAL "riscv")
    target_sources(kernel.elf PRIVATE
        src/arch/riscv/arch.c
        src/arch/riscv/irq.c
        src/arch/riscv/irq.s
        src/arch/riscv/pmp.c
        src/arch/riscv/startup.s
        src/arch/riscv/uspace.s
    )
endif()

target_include_directories(kernel.elf PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_options(kernel.elf PRIVATE
    -T ${CMAKE_CURRENT_SOURCE_DIR}/ld/$ENV{TARGET_MCU}_$ENV{TARGET_ARCH}.ld -nostartfiles
    -Wl,-Map=${CMAKE_BINARY_DIR}/kernel.map,--no-warn-rwx-segments
    -Wl,--defsym=STACK_SIZE=$ENV{RUNTIME_STACK_SIZE}
    -Wl,--defsym=BASE_ADDR=$ENV{MM_BASE_ADDR}
)
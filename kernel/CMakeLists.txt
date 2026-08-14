cmake_minimum_required(VERSION 3.2)

set(CMAKE_ASM_COMPILER riscv32-unknown-elf-gcc)
set(CMAKE_C_COMPILER riscv32-unknown-elf-gcc)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

project(kernel LANGUAGES C ASM)
add_executable(kernel.elf
    src/main.c
    src/arch/riscv/startup.s
)

include_directories(${CMAKE_SOURCE_DIR}/../lib/littlefs/src)
include_directories(${CMAKE_SOURCE_DIR}/../drv)

target_compile_options(kernel.elf PRIVATE -march=rv32ima_zicsr_zifencei_zba_zbb_zbs_zbkb_zca_zcb_zcmp -mabi=ilp32 -Wall -g)
target_link_options(kernel.elf PRIVATE -T ${CMAKE_SOURCE_DIR}/memmap.ld -nostartfiles -Wl,-Map=${CMAKE_BINARY_DIR}/kernel.map)
target_compile_definitions(kernel.elf PRIVATE -DFS_$ENV{FS_TYPE})
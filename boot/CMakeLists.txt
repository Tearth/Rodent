cmake_minimum_required(VERSION 3.2)

set(CMAKE_ASM_COMPILER riscv32-unknown-elf-gcc)
set(CMAKE_C_COMPILER riscv32-unknown-elf-gcc)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

project(bootloader LANGUAGES C ASM)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_compile_options(-march=rv32ima_zicsr_zifencei_zba_zbb_zbs_zbkb_zca_zcb_zcmp -mabi=ilp32 -g)
add_link_options(-T ${CMAKE_SOURCE_DIR}/memmap.ld -nostartfiles)

add_executable(bootloader.elf src/main.c src/image.s)
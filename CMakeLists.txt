cmake_minimum_required(VERSION 3.2)

set(CMAKE_ASM_COMPILER riscv32-unknown-elf-gcc)
set(CMAKE_C_COMPILER riscv32-unknown-elf-gcc)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/drv/src)
include_directories(${CMAKE_SOURCE_DIR}/lib/littlefs/src)

add_compile_options(-march=rv32ima_zicsr_zifencei_zba_zbb_zbs_zbkb_zca_zcb_zcmp -mabi=ilp32 -Wall -g)

string(TOUPPER $ENV{TARGET_CPU} TARGET_CPU_UPPER)
string(TOUPPER $ENV{TARGET_ARCH} TARGET_ARCH_UPPER)
string(TOUPPER $ENV{FS_TYPE} FS_TYPE_UPPER)

add_compile_definitions(
    -DCPU_${TARGET_CPU_UPPER}
    -DARCH_${TARGET_ARCH_UPPER}
    -DFS_${FS_TYPE_UPPER}
)

project(picos)
add_subdirectory(drv)
add_subdirectory(boot)
add_subdirectory(kernel)
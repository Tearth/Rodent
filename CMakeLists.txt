cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW)

set(CMAKE_ASM_COMPILER riscv32-unknown-elf-gcc)
set(CMAKE_C_COMPILER riscv32-unknown-elf-gcc)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_C_FLAGS "-march=rv32ima_zicsr_zifencei_zba_zbb_zbs_zbkb_zca_zcb_zcmp -mabi=ilp32 -Wall")
set(CMAKE_C_FLAGS_DEBUG "-Og -g")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG FALSE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

include_directories(${CMAKE_SOURCE_DIR}/drv/src)
include_directories(${CMAKE_SOURCE_DIR}/lib/littlefs/src)

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
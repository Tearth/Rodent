cmake_minimum_required(VERSION 3.2)

set(CMAKE_ASM_COMPILER riscv32-unknown-elf-gcc)
set(CMAKE_C_COMPILER riscv32-unknown-elf-gcc)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

project(bootloader LANGUAGES C ASM)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/lib/littlefs/src)

add_compile_options(-march=rv32ima_zicsr_zifencei_zba_zbb_zbs_zbkb_zca_zcb_zcmp -mabi=ilp32 -Wall -g)
add_link_options(-T ${CMAKE_SOURCE_DIR}/memmap.ld -nostartfiles -Wl,-Map=${CMAKE_BINARY_DIR}/bootloader.map)
add_definitions(-DFS_$ENV{FS_TYPE})

set (FS_BASE_ADDR $ENV{FS_BASE_ADDR})
set (KERNEL_FILENAME $ENV{KERNEL_FILENAME})

configure_file(${CMAKE_SOURCE_DIR}/src/cfg/config.h.in ${CMAKE_SOURCE_DIR}/src/cfg/config.h)

if($ENV{FS_TYPE} STREQUAL "LFS")
    add_definitions(
        -DFS_LFS
        -DLFS_NO_MALLOC 
        -DLFS_NO_ASSERT 
        -DLFS_NO_DEBUG 
        -DLFS_NO_WARN 
        -DLFS_NO_ERROR 
        -DLFS_READONLY
    )
endif()

add_executable(
    bootloader.elf
    src/main.c
    src/fs/fs_lfs.c
    src/asm/image.s
    src/asm/startup.s
    lib/littlefs/src/lfs.c
    lib/littlefs/src/lfs_util.c
)
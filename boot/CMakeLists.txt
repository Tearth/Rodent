cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW)
project(bootloader LANGUAGES C ASM)

add_executable(bootloader.elf
    $<TARGET_OBJECTS:mcu>
    src/main.c
    src/log.c
)

target_include_directories(bootloader.elf PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_options(bootloader.elf PRIVATE
    -T ${CMAKE_CURRENT_SOURCE_DIR}/ld/$ENV{TARGET_MCU}_$ENV{TARGET_ARCH}.ld -nostartfiles
    -Wl,-Map=${CMAKE_BINARY_DIR}/bootloader.map
)

if($ENV{TARGET_MCU} STREQUAL "rp2350")
    target_sources(bootloader.elf PRIVATE
        src/mcu/rp2350/image_$ENV{TARGET_ARCH}.s
        src/mcu/rp2350/startup_$ENV{TARGET_ARCH}.s
        src/mcu/mcu_rp2350.c
    )
endif()

if($ENV{TARGET_ARCH} STREQUAL "riscv")
    target_sources(bootloader.elf PRIVATE src/arch/arch_riscv.c)
endif()

if($ENV{FS_TYPE} STREQUAL "lfs")
    target_sources(bootloader.elf PRIVATE
        src/fs/fs_lfs.c
        ../lib/littlefs/src/lfs.c
        ../lib/littlefs/src/lfs_util.c
    )

    target_compile_definitions(bootloader.elf PRIVATE
        LFS_NO_MALLOC
        LFS_NO_ASSERT
        LFS_NO_DEBUG
        LFS_NO_WARN
        LFS_NO_ERROR
        LFS_READONLY
    )
endif()

set(FS_BASE_ADDR $ENV{FS_BASE_ADDR})
set(KERNEL_FILENAME $ENV{KERNEL_FILENAME})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cfg/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cfg/config.h
)
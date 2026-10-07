cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW)
project(bootloader LANGUAGES C ASM)

add_executable(bootloader.elf
    $<TARGET_OBJECTS:mcu>
    src/main.c
    src/elf.c
    src/log.c
    src/cfg.c
    src/fs.c
)

target_include_directories(bootloader.elf BEFORE PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_options(bootloader.elf PRIVATE
    -T ${CMAKE_CURRENT_SOURCE_DIR}/ld/$ENV{TARGET_MCU}_$ENV{TARGET_ARCH}.ld -nostartfiles
    -Wl,-Map=${CMAKE_BINARY_DIR}/bootloader.map
)

if($ENV{TARGET_ARCH} STREQUAL "riscv")
    target_sources(bootloader.elf PRIVATE src/arch/riscv.c)
endif()

if($ENV{TARGET_MCU} STREQUAL "rp2350")
    target_sources(bootloader.elf PRIVATE
        src/mcu/rp2350/$ENV{TARGET_ARCH}/image.s
        src/mcu/rp2350/$ENV{TARGET_ARCH}/startup.s
        src/mcu/rp2350/clk.c
        src/mcu/rp2350/flash.c
        src/mcu/rp2350/mcu.c
        src/mcu/rp2350/uart.c
    )
endif()

if($ENV{FS_TYPE} STREQUAL "lfs")
    target_sources(bootloader.elf PRIVATE
        src/fs/lfs.c
        ../lib/littlefs/lfs.c
        ../lib/littlefs/lfs_util.c
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
set(BOOT_CFG $ENV{BOOT_CFG})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/def.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/src/def.h
)
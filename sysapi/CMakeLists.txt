cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW)
project(sysapi LANGUAGES C ASM)

add_library(sysapi OBJECT
    src/rodent/thread.c
)

target_include_directories(sysapi BEFORE PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if($ENV{TARGET_ARCH} STREQUAL "riscv")
    target_sources(sysapi PRIVATE
        src/arch/riscv.c
    )

    target_compile_options(sysapi PRIVATE
        -mcmodel=medany -fPIC
    )
endif()

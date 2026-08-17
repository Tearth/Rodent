cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW)
project(hal LANGUAGES C ASM)

if($ENV{TARGET_CPU} STREQUAL "rp2350")
    add_library(hal OBJECT
        $<TARGET_OBJECTS:drv>
        src/hal_rp2350.c
    )
endif()

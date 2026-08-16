cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW)
project(drv LANGUAGES C ASM)

if($ENV{TARGET_CPU} STREQUAL "rp2350")
    add_library(drv OBJECT
        src/rp2350/clock.c
        src/rp2350/gpio.c
        src/rp2350/reset.c
        src/rp2350/timer.c
        src/rp2350/uart.c
    )
endif()

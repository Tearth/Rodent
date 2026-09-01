cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW)
project(mcu LANGUAGES C ASM)

if($ENV{TARGET_MCU} STREQUAL "rp2350")
    add_library(mcu OBJECT
        src/rp2350/clock.c
        src/rp2350/gpio.c
        src/rp2350/qmi.c
        src/rp2350/reset.c
        src/rp2350/timer.c
        src/rp2350/uart.c
    )

    target_compile_options(mcu PRIVATE
        -mcmodel=medany -fPIE
    )
endif()

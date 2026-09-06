cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW)
project(app2.elf LANGUAGES C ASM)

add_executable(app2.elf
    $<TARGET_OBJECTS:mcu>
    src/main.c
)

target_include_directories(app2.elf PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(app2.elf PRIVATE
    -mcmodel=medany -fPIE
)

target_link_options(app2.elf PRIVATE
    -T ${CMAKE_CURRENT_SOURCE_DIR}/ld/app.ld -nostartfiles
    -Wl,-Map=${CMAKE_BINARY_DIR}/app2.map,--no-warn-rwx-segments
    -Wl,--defsym=STACK_SIZE=$ENV{RUNTIME_STACK_SIZE}
    -Wl,--defsym=CELL_SIZE=$ENV{MM_CELL_SIZE}
)
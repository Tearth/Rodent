cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-Os")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG FALSE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

include_directories(${CMAKE_SOURCE_DIR}/mcu/src)
include_directories(${CMAKE_SOURCE_DIR}/lib/littlefs/src)
include_directories(${CMAKE_SOURCE_DIR}/shared/src)
include_directories(${LIBC_PATH}/include)

link_directories(${LIBC_PATH}/lib)
link_libraries(libc.a)

string(TOUPPER $ENV{TARGET_MCU} TARGET_MCU_UPPER)
string(TOUPPER $ENV{TARGET_ARCH} TARGET_ARCH_UPPER)
string(TOUPPER $ENV{FS_TYPE} FS_TYPE_UPPER)

add_compile_definitions(
    MCU_${TARGET_MCU_UPPER}
    ARCH_${TARGET_ARCH_UPPER}
    FS_${FS_TYPE_UPPER}
)

project(rodent)
add_subdirectory(mcu)
add_subdirectory(boot)
add_subdirectory(kernel)
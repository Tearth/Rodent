cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0069 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-Os")
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

project(rodent)
add_subdirectory(drv)
add_subdirectory(boot)
add_subdirectory(kernel)
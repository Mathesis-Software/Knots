cmake_minimum_required(VERSION 3.19)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

find_program(CCACHE_EXECUTABLE ccache)

if(CCACHE_EXECUTABLE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE} CACHE STRING "")
endif()

project(KnotEditor)

include("${CMAKE_SOURCE_DIR}/vendor/rapidjson.cmake")
include_directories(${RAPIDJSON_INCLUDE_DIR})

include (cmake/win32.cmake)

add_subdirectory(src/ke)
add_subdirectory(src/math)
add_subdirectory(src/other)
add_subdirectory(src/qt)

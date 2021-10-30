cmake_minimum_required(VERSION 3.19)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH on)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

find_program(CCACHE_EXECUTABLE ccache)

if(CCACHE_EXECUTABLE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE} CACHE STRING "")
endif()

project(KnotEditor)

include("${CMAKE_SOURCE_DIR}/vendor/rapidjson.cmake")

include (win32)

add_subdirectory(modules/core)
add_subdirectory(modules/math)
add_subdirectory(modules/other)
add_subdirectory(modules/app)

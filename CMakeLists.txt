cmake_minimum_required(VERSION 3.14)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(KnotEditor)

include("${CMAKE_SOURCE_DIR}/vendor/rapidjson.cmake")
include_directories(${RAPIDJSON_INCLUDE_DIR})


add_subdirectory( src/ke )
add_subdirectory( src/math )
add_subdirectory( src/other )

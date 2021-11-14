project(math)

file(GLOB SOURCES src/*.cpp)
include_directories(../core/src)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_dependencies(${PROJECT_NAME} core)

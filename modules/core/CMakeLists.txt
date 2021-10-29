project(core)

file(GLOB SOURCES src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_dependencies(${PROJECT_NAME} rapidjson)

project(core)

file(GLOB SOURCES src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# Add explicit dependency on RapidJSON external target to download it
if (NOT ${RapidJSON_FOUND})
    add_dependencies(${PROJECT_NAME} RapidJSON)
endif()
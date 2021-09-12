project (torus)

file(GLOB SOURCES "*.cpp" )

add_executable( ${PROJECT_NAME} ${SOURCES})
add_dependencies( ${PROJECT_NAME} rapidjson math ke)
target_link_libraries(${PROJECT_NAME} ke math)


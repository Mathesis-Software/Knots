project (alexander_polynomial)
include_directories( ../../ke ../../math)

file(GLOB SOURCES "*.cpp" )

add_executable( ${PROJECT_NAME} ${SOURCES})
add_dependencies( ${PROJECT_NAME} rapidjson)
target_link_libraries(${PROJECT_NAME} ke math)


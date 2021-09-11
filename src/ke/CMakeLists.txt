project(ke)


file(GLOB SOURCES "*.cpp" )

add_library( ${PROJECT_NAME} STATIC ${SOURCES} )
add_dependencies( ${PROJECT_NAME} rapidjson)


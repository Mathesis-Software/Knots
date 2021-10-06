project(math)

file(GLOB SOURCES "*.cpp" )
add_library( ${PROJECT_NAME} STATIC ${SOURCES} )

project(math)


#INCLUDE_DIRECTORIES( "third_party/tree-sitter/lib/include" 
#                     "third_party/tree-sitter/lib/src" 
#                     "third_party/tree-sitter/lib/utf8proc"
#                     "third_party/xxHash")


file(GLOB SOURCES "*.cpp" )

add_library( ${PROJECT_NAME} STATIC ${SOURCES} )

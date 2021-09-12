if (MSVC)
add_libraries(OpenGL32)
add_definitions(-D _CRT_SECURE_NO_WARNINGS)
add_definitions(-D _USE_MATH_DEFINES)
endif()


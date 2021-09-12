if (WIN32)

	link_libraries(OpenGL32)

	if(MSVC)
		add_definitions(-D _CRT_SECURE_NO_WARNINGS)
		add_definitions(-D _USE_MATH_DEFINES)
	endif()
endif()


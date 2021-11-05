if (WIN32)

	link_libraries(OpenGL32)

	if(MSVC)
		add_definitions(-D _CRT_SECURE_NO_WARNINGS)
		add_definitions(-D _USE_MATH_DEFINES)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:8388608")
	elseif(MINGW)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,8388608")
	endif()

endif()
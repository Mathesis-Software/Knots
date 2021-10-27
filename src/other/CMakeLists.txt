project (other)

add_executable(torus torus.cpp)
add_dependencies(torus rapidjson ke)
target_link_libraries(torus ke)

add_executable(alexander_polynomial alexander_polynomial.cpp)
add_dependencies(alexander_polynomial rapidjson ke math)
target_link_libraries(alexander_polynomial ke math)

add_executable(converter converter.cpp)
add_dependencies(converter rapidjson ke)
target_link_libraries(converter ke)

add_executable(dtcode dtcode.cpp)
add_dependencies(dtcode rapidjson ke math)
target_link_libraries(dtcode ke math)

add_executable(vassiliev vassiliev.cpp)
add_dependencies(vassiliev rapidjson ke math)
target_link_libraries(vassiliev ke math)

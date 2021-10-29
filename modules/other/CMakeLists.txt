project (other)

include_directories(../core/src)
include_directories(../math/src)

add_executable(torus src/torus.cpp)
add_dependencies(torus rapidjson core)
target_link_libraries(torus core)

add_executable(alexander_polynomial src/alexander_polynomial.cpp)
add_dependencies(alexander_polynomial rapidjson core math)
target_link_libraries(alexander_polynomial core math)

add_executable(converter src/converter.cpp)
add_dependencies(converter rapidjson core)
target_link_libraries(converter core)

add_executable(dtcode src/dtcode.cpp)
add_dependencies(dtcode rapidjson core math)
target_link_libraries(dtcode core math)

add_executable(vassiliev src/vassiliev.cpp)
add_dependencies(vassiliev rapidjson core math)
target_link_libraries(vassiliev core math)

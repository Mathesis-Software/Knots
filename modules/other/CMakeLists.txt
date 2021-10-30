project (other)

include_directories(../core/src)
include_directories(../math/src)

set(targets "torus;alexander_polynomial;converter;dtcode;vassiliev")

foreach(target ${targets})
    add_executable(${target} src/${target}.cpp)
    add_dependencies(${target} rapidjson)
    target_link_libraries(${target} core math)
endforeach()

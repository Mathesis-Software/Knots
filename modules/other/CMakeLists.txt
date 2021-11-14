project (other)

include_directories(../core/src)
include_directories(../math/src)

set(targets "torus;alexander_polynomial;converter;dtcode;vassiliev")

foreach(target ${targets})
    add_executable(${target} src/${target}.cpp)
    add_dependencies(${target} core)
    target_link_libraries(${target} core)
endforeach()

set(math_dependent "alexander_polynomial;dtcode;vassiliev")

foreach(target ${math_dependent})
    add_dependencies(${target} math)
    target_link_libraries(${target} math)
endforeach()

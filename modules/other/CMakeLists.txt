project (other)

include_directories(../core/src)
include_directories(../math/src)

set(targets "torus;alexander_polynomial;converter;dtcode;vassiliev")

foreach(target ${targets})
    add_executable(${target} src/${target}.cpp)
    target_link_libraries(${target} core)
endforeach()

set(math_dependent "alexander_polynomial;dtcode;vassiliev")

foreach(target ${math_dependent})
    target_link_libraries(${target} math)
endforeach()

set(CMAKE_CXX_STANDARD 17)

add_library(realization1 SHARED src/realization1.cpp)
add_library(realization2 SHARED src/realization2.cpp)

target_include_directories(realization1 PUBLIC include)
target_include_directories(realization2 PUBLIC include)

add_executable(static_main static_main.cpp)
target_link_libraries(static_main PRIVATE realization1)

add_executable(dynamic_main dynamic_main.cpp include/utils.hpp src/utils.cpp)
target_link_libraries(dynamic_main dl)
target_include_directories(dynamic_main PRIVATE include)
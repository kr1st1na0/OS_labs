set(CMAKE_CXX_STANDARD 17)

add_executable(lab3 include/utils.hpp
        main.cpp
        src/parent.cpp
        src/utils.cpp)

target_include_directories(lab3 PRIVATE include)

add_executable(child3 include/utils.hpp
        src/child.cpp
        src/utils.cpp)

target_include_directories(child3 PRIVATE include)

find_package(Threads REQUIRED)
target_link_libraries(child3 PRIVATE pthread)
target_link_libraries(child3 PRIVATE rt)

target_link_libraries(lab3 PRIVATE pthread)
target_link_libraries(lab3 PRIVATE rt)

add_dependencies(lab3 child3)
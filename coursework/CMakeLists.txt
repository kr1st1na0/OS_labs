set(CMAKE_CXX_STANDARD 17)

add_library(cw_lib src/graph.cpp 
        src/dag.cpp 
        src/parser.cpp 
        src/executor.cpp
)

target_include_directories(cw_lib PUBLIC lib include)

add_executable(cw_main main.cpp)

target_link_libraries(cw_main PUBLIC cw_lib)
target_link_libraries(cw_main PRIVATE Threads::Threads)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-lzmq")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_compile_options(-Wno-error=deprecated-declarations -Wdeprecated-declarations)

add_executable(client include/nodeRoutine.hpp include/socketRoutine.hpp
        src/nodeRoutine.cpp src/socketRoutine.cpp src/client.cpp
)
target_include_directories(client PRIVATE include)

add_executable(server include/nodeRoutine.hpp include/socketRoutine.hpp
        src/nodeRoutine.cpp src/socketRoutine.cpp src/server.cpp
)
target_include_directories(server PRIVATE include)

target_link_libraries(client PRIVATE zmq)

target_link_libraries(server PRIVATE zmq)
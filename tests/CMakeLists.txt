include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

set(CMAKE_CXX_STANDARD 17)

# Lab1
add_executable(lab1_test lab1_test.cpp ../lab1/src/parent.cpp ../lab1/include/parent.hpp
        ../lab1/src/utils.cpp ../lab1/include/utils.hpp)

target_link_libraries(lab1_test PRIVATE GTest::GTest)
target_include_directories(lab1_test PRIVATE ../lab1/include)

add_test(lab1_test lab1_test)

# Lab2
add_executable(lab2_test lab2_test.cpp ../lab2/src/lab2.cpp ../lab2/include/lab2.hpp)

target_link_libraries(lab2_test PRIVATE GTest::GTest)
target_link_libraries(lab2_test PRIVATE Threads::Threads)

target_include_directories(lab2_test PRIVATE ../lab2/include)

add_test(lab2_test lab2_test)

# Lab3
add_executable(lab3_test lab3_test.cpp ../lab3/src/parent.cpp ../lab3/include/parent.hpp
        ../lab3/src/utils.cpp ../lab3/include/utils.hpp)

target_link_libraries(lab3_test PRIVATE GTest::GTest)
target_include_directories(lab3_test PRIVATE ../lab3/include)
target_link_libraries(lab3_test PRIVATE pthread)
target_link_libraries(lab3_test PRIVATE rt)

add_test(lab3_test lab3_test)

# Lab4
add_executable(lab4_test1 lab4_test1.cpp ../lab4/include/realizations.hpp 
        ../lab4/src/realization1.cpp)
target_link_libraries(lab4_test1 PRIVATE GTest::GTest)
target_include_directories(lab4_test1 PRIVATE ../lab4/include)

add_test(lab4_test1 lab4_test1)

add_executable(lab4_test2 lab4_test2.cpp ../lab4/include/realizations.hpp 
        ../lab4/src/realization2.cpp)
target_link_libraries(lab4_test2 PRIVATE GTest::GTest)
target_include_directories(lab4_test2 PRIVATE ../lab4/include)

add_test(lab4_test2 lab4_test2)

# Lab 5-7

set(CMAKE_CXX_FLAGS "-lzmq")
add_compile_options(-Wno-error=deprecated-declarations -Wdeprecated-declarations)

add_executable(lab5-7_test lab5-7_test.cpp ../lab5-7/src/nodeRoutine.cpp ../lab5-7/src/socketRoutine.cpp)
target_link_libraries(lab5-7_test PRIVATE GTest::GTest)
target_include_directories(lab5-7_test PRIVATE ../lab5-7/include)

target_link_libraries(lab5-7_test PRIVATE zmq)

add_test(lab5-7_test lab5-7_test)
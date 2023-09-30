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
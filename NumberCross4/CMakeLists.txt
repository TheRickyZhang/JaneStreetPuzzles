cmake_minimum_required(VERSION 3.27)
project(Number_Cross_4)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_executable(Number_Cross_4 main.cpp
        grid.cpp
        grid.h)

enable_testing()

add_executable(Number_Cross_4_tests
        test_main.cpp
        grid.cpp
        grid.h)

target_link_libraries(
        Number_Cross_4_tests
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(Number_Cross_4_tests)


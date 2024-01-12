cmake_minimum_required(VERSION 3.27)
project(SomeFSquares)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_executable(SomeFSquares main.cpp
        Pentomino.cpp)

enable_testing()

add_executable(
        SomeFSquares_tests
        test_main.cpp
        Pentomino.cpp
)
target_link_libraries(
        SomeFSquares_tests
        gtest_main
)
include(GoogleTest)
gtest_discover_tests(SomeFSquares_tests)


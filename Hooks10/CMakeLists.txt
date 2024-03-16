cmake_minimum_required(VERSION 3.27)
project(Hooks10)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_executable(Hooks10 main.cpp Grid.cpp)

enable_testing()

add_executable(Hooks10_tests test_main.cpp Grid.cpp)
target_link_libraries(Hooks10_tests gtest_main)

include(GoogleTest)
gtest_discover_tests(Hooks10_tests)

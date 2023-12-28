cmake_minimum_required(VERSION 3.20)
project(MirrorsTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# Add the path to the library
link_directories(${CMAKE_SOURCE_DIR}/libs)

# Add your test executable
add_executable(MirrorsTest tests/test_main.cpp)

# Link the test executable with Google Test and the hall_of_mirrors_lib
target_link_libraries(MirrorsTest gtest_main hall_of_mirrors_lib)

# Include the directory where hall_of_mirrors_lib headers are located
# Replace with the actual path to the headers
target_include_directories(MirrorsTest PRIVATE path/to/HallOfMirrors/include)

enable_testing()
add_test(NAME HallOfMirrorsTests COMMAND MirrorsTest)

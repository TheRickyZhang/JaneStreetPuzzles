cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(HallOfMirrors)
set(SFML_DIR "C:/Libraries/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Create a library with the core functionalities
add_library(hall_of_mirrors_lib
        Pixels.cpp
        Reflection.cpp
)

target_link_libraries(hall_of_mirrors_lib
        sfml-graphics
        sfml-window
        sfml-system
)

# Create the main executable
add_executable(HallOfMirrors main.cpp)

# Link the main executable with your library and any other needed libraries
target_link_libraries(HallOfMirrors
        hall_of_mirrors_lib
)


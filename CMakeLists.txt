cmake_minimum_required(VERSION 3.10)
project(julia-sets-render VERSION 0.1.0 LANGUAGES CXX)

find_package(SFML COMPONENTS graphics window system REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(julia-sets-render main.cpp)

target_link_libraries(julia-sets-render
  PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    OpenGL::GL
)

cmake_minimum_required(VERSION 3.10)
project(julia-sets-render VERSION 0.1.0 LANGUAGES CXX)

find_package(SFML COMPONENTS graphics window system REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenMP)

add_executable(julia-sets-render main.cpp)

target_link_libraries(julia-sets-render
  PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    OpenGL::GL
)

target_compile_features(julia-sets-render
  PRIVATE
    cxx_std_17
)
target_compile_options(julia-sets-render
  PRIVATE
    -O3 -march=native -ftree-vectorize
)
if (OPENMP_CXX_FOUND)
  target_link_libraries(julia-sets-render
    PRIVATE
    OpenMP::OpenMP_CXX
  )
endif()

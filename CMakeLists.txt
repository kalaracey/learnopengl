cmake_minimum_required(VERSION 3.22)
project(learnopengl)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -pedantic -Werror)

find_package(glfw3 REQUIRED)

include_directories(glad/include)

add_library(glad glad/src/glad.c)

link_libraries(glfw glad)

add_executable(hellowindow hellowindow.cpp)
add_executable(hellotriangle hellotriangle.cpp)
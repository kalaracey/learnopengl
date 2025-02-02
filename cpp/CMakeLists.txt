cmake_minimum_required(VERSION 3.22)
project(learnopengl)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -pedantic -Werror)

find_package(glfw3 REQUIRED)

include_directories(glad/include)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_library(glad glad/src/glad.c)

link_libraries(glfw glad)

add_library(common common.cpp)
add_library(shader shader.cc)
link_libraries(common shader)

add_executable(hellowindow hellowindow.cpp)
add_executable(hellotriangle hellotriangle.cpp)
add_executable(shaders shaders.cpp)

add_library(stb_image third_party/stb_image.cpp)
link_libraries(stb_image)
add_executable(textures textures.cpp)

find_package(GLM REQUIRED)
include_directories("${GLM_INCLUDE_DIRS}")
link_libraries(glm::glm)
add_executable(transformations transformations.cpp)


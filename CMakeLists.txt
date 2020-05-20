cmake_minimum_required(VERSION 3.16)
project(opengl_pathtracer)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} glfw)
add_executable(opengl_pathtracer main.cpp)
target_link_libraries(opengl_pathtracer ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw)
cmake_minimum_required(VERSION 3.15)
project(glsl-viewer)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

find_package(glfw3 REQUIRED)
include_directories("thirdparty/glad-46/include/")
file(GLOB SRC "*.h" "src/*.cpp" "src/*/*.cpp" "thirdparty/glad-46/src/glad.c")

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} glfw opengl32)
cmake_minimum_required(VERSION 3.15)

project(Pong VERSION 1.0 LANGUAGES C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(raylib REQUIRED)

add_executable(Pong src/main.c)
target_link_libraries(Pong raylib)
target_compile_options(Pong PRIVATE -Wall -Wextra)
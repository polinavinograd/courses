cmake_minimum_required(VERSION 3.16)

set(CMAKE_BUILD_TYPE Debug)

project(courses CXX)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(01)
add_subdirectory(02-sdl-hw)
add_subdirectory(03-sdl-engine-hw)



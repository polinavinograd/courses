cmake_minimum_required(VERSION 3.20)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  message(FATAL_ERROR "Please create a build directory outside of the source code")
endif()

project(sdl-shared CXX)

file(
    GLOB
    CXX_SRC
    =
    "*.cxx")
add_executable(${PROJECT_NAME} ${CXX_SRC})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
add_definitions(-Wall -O2)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)

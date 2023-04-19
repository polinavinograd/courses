cmake_minimum_required(VERSION 3.26)
set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(
        FATAL_ERROR "Please create a build directory outside of the source code"
        )
endif()

project(sdl-engine-2 CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
add_definitions(-O2)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
add_library(${PROJECT_NAME}_lib SHARED engine.cxx engine.hxx texture.hxx)
target_link_libraries(${PROJECT_NAME}_lib PRIVATE SDL3::SDL3-shared
                                                  SDL3_image::SDL3_image)
target_include_directories(${PROJECT_NAME}_lib PUBLIC CMAKE_CURRENT_SOURCE_DIR)

add_executable(${PROJECT_NAME} main.cxx)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)

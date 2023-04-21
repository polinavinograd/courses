cmake_minimum_required(VERSION 3.26)
set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(
        FATAL_ERROR "Please create a build directory outside of the source code"
        )
endif()

project(sdl-engine-3 CXX)

add_definitions(-O2)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
add_library(game_lib SHARED engine.hxx game.cxx texture.hxx)
target_include_directories(game_lib PUBLIC CMAKE_CURRENT_SOURCE_DIR)

add_executable(${PROJECT_NAME} engine.cxx engine.hxx)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE game_lib SDL3::SDL3-shared
                                              SDL3_image::SDL3_image)

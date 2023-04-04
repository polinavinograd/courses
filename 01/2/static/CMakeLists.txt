cmake_minimum_required(VERSION 3.0.0)
project(hello_static CXX)

# set the latest c++ standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

# enable all warnings, defines optimisation level
add_definitions(-Wall -O2)

# create shared library
add_library(${PROJECT_NAME}_lib STATIC src/hello.cpp)
# add_library(${PROJECT_NAME}_lib::hello_dynamic ALIAS ${PROJECT_NAME}_lib)

target_include_directories(${PROJECT_NAME}_lib PUBLIC src)

# create executable file
file(GLOB
    CPP_SOURCE "*.cpp")

add_executable(${PROJECT_NAME} ${CPP_SOURCE})

# link library to file
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)

include(CTest)

add_test(NAME test_hello_static COMMAND hello_static)

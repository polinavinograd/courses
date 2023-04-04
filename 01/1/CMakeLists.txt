cmake_minimum_required(VERSION 3.0.0)
project(hello_world CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(hello_world hello_world.cxx)

include(CTest)

add_test(NAME test_hello_world_1 COMMAND hello_world)

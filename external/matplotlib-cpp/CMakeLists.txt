cmake_minimum_required(VERSION 3.10)
project(matplotlib_cpp)
add_library(matplotlib_cpp INTERFACE)
target_include_directories(matplotlib_cpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

cmake_minimum_required(VERSION 3.21)
project(Ecosym)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES src/*.cpp)
add_executable(ecosym ${SOURCES})
target_include_directories(ecosym PRIVATE include/ecosym)

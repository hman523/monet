cmake_minimum_required(VERSION 3.13)
project(BIPL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
add_executable(BIPL main.cpp Interpreter.cpp Interpreter.h)
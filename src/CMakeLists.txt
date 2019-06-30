cmake_minimum_required(VERSION 3.13)
project(BIPL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror=format-security -Werror=implicit-function-declaration -fexceptions")
add_executable(Monet main.cpp Interpreter.cpp Interpreter.h Memory.cpp Memory.h)
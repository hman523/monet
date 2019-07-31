cmake_minimum_required(VERSION 3.13)
project(MONET)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror=format-security -Werror=implicit-function-declaration -fexceptions -O2")
add_executable(Monet src/main.cpp src/Interpreter.cpp src/Interpreter.h src/Memory.cpp src/Memory.h src/Exception.cpp src/Exception.h)
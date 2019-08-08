cmake_minimum_required(VERSION 3.13)
project(MONET)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror=format-security -Werror=implicit-function-declaration -fexceptions -O2")
add_executable(Monet src/main.cpp src/Interpreter.cpp include/Interpreter.h src/Memory.cpp include/Memory.h src/Exception.cpp include/Exception.h lib/Library.h lib/File/File.cpp lib/File/File.h lib/Time/Time.cpp lib/Time/Time.h lib/Math/Math.cpp lib/Math/Math.h)
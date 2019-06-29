#include "Interpreter.h"
#include <iostream>

// Welcome to BIPL (Basic Inspired Programming Language)

int main(int argc, char *argv[]) {
  std::cout << "Welcome to the BIPL Interpreter" << std::endl;
  if (argc != 2) {
    std::cerr << "Need a file to interpret" << std::endl;
    exit(1);
  }
  Interpreter i(argv[1]);

  return 0;
}
#include "Interpreter.h"
#include <iostream>

// Welcome to Monet (A Basic Inspired Programming Language)

int main(int argc, char *argv[]) {
  std::cout << "Welcome to the Monet Interpreter" << std::endl;
  if (argc != 2) {
    std::cerr << "Need a file to interpret" << std::endl;
    exit(1);
  }
  try {
    Interpreter i(argv[1]);
  } catch (std::logic_error e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
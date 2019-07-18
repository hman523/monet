#include "Interpreter.h"
#include <iostream>

// Welcome to Monet (A Basic Inspired Programming Language)

/**
 * Main function
 * @param argc number of cmd args
 * @param argv cmd args
 * @return 0 on success, else on failure
 */
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
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
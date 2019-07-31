/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Main.cpp
 */

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
  if (argc > 2) {
    std::cerr << "Need a file to interpret or no command line parameter to "
                 "enter the REPL"
              << std::endl;
    exit(1);
  }
  try {
    if (argc == 1) {
      Interpreter::Instance();
    } else if (argc == 2) {
      Interpreter::Instance(argv[1]);
    }
  } catch (Exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
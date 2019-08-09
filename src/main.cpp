/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Main.cpp
 */

#include "../include/Interpreter.h"
#include <iostream>

// Welcome to Monet (A Basic Inspired Programming Language)

/**
 * Main function
 * @param argc number of cmd args
 * @param argv cmd args
 * @return 0 on success, else on failure
 */
int main(int argc, char *argv[]) {
  if (argc > 2) {
    std::cerr << "Need a file to interpret or no command line parameter to "
                 "enter the REPL"
              << std::endl;
    exit(1);
  }
  try {
    if (argc == 1) {
      std::cout << "Welcome to the Monet Interpreter" << std::endl;
      Interpreter *i = Interpreter::Instance();
      i->repl();
    } else if (argc == 2) {
      Interpreter *i = Interpreter::Instance(argv[1]);
      i->interpret();
    }
  } catch (Exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
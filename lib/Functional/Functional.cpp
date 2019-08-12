/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Functional.cpp
 */

#include "Functional.h"
#include "../../include/Interpreter.h"
#include <vector>

Functional::Functional() : functions({"f.map", "f.apply", "f.reduce"}) {}

std::set<std::string> Functional::getFunctions() { return functions; }

std::string Functional::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "f.map") {
    return map(words);
  } else if (words[0] == "f.apply") {
    return apply(words);
  } else if (words[0] == "f.reduce") {
    return reduce(words);
  }
  throw Exception("A fatal implementation error has occurred in the Functional "
                  "library in Functional.cpp");
}

std::string Functional::map(const std::vector<std::string> &expression) {
  return std::__cxx11::string();
}

std::string Functional::apply(const std::vector<std::string> &expression) {
  return std::__cxx11::string();
}

std::string Functional::reduce(const std::vector<std::string> &expression) {
  return std::__cxx11::string();
}

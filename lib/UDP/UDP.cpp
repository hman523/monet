//
// Created by hunter on 11/18/19.
//

#include "UDP.h"
#include "../../include/Interpreter.h"

UDP::UDP() { functions.insert({}); }

std::set<std::string> UDP::getFunctions() { return functions; }

std::string UDP::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "") {

  } else if (words[0] == "") {

  } else if (words[0] == "") {
  }
  throw Exception("Fatal implementation error in UDP library in UDP.cpp");
}
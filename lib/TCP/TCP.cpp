//
// Created by hunter on 11/18/19.
//

#include "TCP.h"
#include "../../include/Interpreter.h"

TCP::TCP() { functions.insert({}); }

std::set<std::string> TCP::getFunctions() { return functions; }

std::string TCP::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "") {

  } else if (words[0] == "") {

  } else if (words[0] == "") {
  }
  throw Exception("Fatal implementation error in TCP library in TCP.cpp");
}

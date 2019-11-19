//
// Created by hunter on 11/18/19.
//

#include "TCP.h"
#include "../../include/Interpreter.h"

TCP::TCP() { functions.insert({"tcp.send", "tcp.receive"}); }

std::set<std::string> TCP::getFunctions() { return functions; }

std::string TCP::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "tcp.send") {
    return send(words);
  } else if (words[0] == "tcp.receive") {
    return rec(words);
  }
  throw Exception("Fatal implementation error in TCP library in TCP.cpp");
}

std::string TCP::send(const std::vector<std::string> &expression) {
  return std::__cxx11::string();
}

std::string TCP::rec(const std::vector<std::string> &expression) {
  return std::__cxx11::string();
}

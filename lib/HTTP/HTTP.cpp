//
// Created by hunter on 11/18/19.
//

#include "HTTP.h"
#include "../../include/Exception.h"
#include "../../include/Interpreter.h"

HTTP::HTTP() { functions.insert({"http.get"}); }

std::set<std::string> HTTP::getFunctions() { return functions; }

std::string HTTP::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "http.get") {
    return get(words);
  } else if (words[0] == "") {

  } else if (words[0] == "") {
  }
  throw Exception("Fatal implementation error in HTTP library in HTTP.cpp");
}

std::string HTTP::get(const std::vector<std::string> &vals) {
  return std::__cxx11::string();
}

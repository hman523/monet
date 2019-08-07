//
// Created by hunter on 8/6/19.
//

#include "Time.h"
#include "../../include/Exception.h"
#include "../../include/Interpreter.h"

Time::Time() {
  functions.insert(
      {"time.time", "time.date", "time.local", "time.list", "time.string"});
}

std::set<std::string> Time::getFunctions() { return functions; }

std::string Time::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "time.time") {
    return time(words);
  } else if (words[0] == "time.date") {
    return date(words);
  } else if (words[0] == "time.local") {
    return local(words);
  } else if (words[0] == "time.list") {
    return list(words);
  } else if (words[0] == "time.string") {
    return stringtime(words);
  }
  throw Exception("Fatal implementation error in Time library in Time.cpp");
}

std::string Time::time(const std::vector<std::string> &vals) {
  return std::__cxx11::string();
}

std::string Time::date(const std::vector<std::string> &vals) {
  return std::__cxx11::string();
}

std::string Time::local(const std::vector<std::string> &vals) {
  return std::__cxx11::string();
}

std::string Time::list(const std::vector<std::string> &vals) {
  return std::__cxx11::string();
}

std::string Time::stringtime(const std::vector<std::string> &vals) {
  return std::__cxx11::string();
}

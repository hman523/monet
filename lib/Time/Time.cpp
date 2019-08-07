/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Time.cpp
 */

#include "Time.h"
#include "../../include/Exception.h"
#include "../../include/Interpreter.h"
#include <ctime>

Time::Time() {
  functions.insert({"time.time", "time.utcdate", "time.local", "time.localdate",
                    "time.list", "time.string"});
}

std::set<std::string> Time::getFunctions() { return functions; }

std::string Time::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "time.time") {
    return time(words);
  } else if (words[0] == "time.utcdate") {
    return date(words);
  } else if (words[0] == "time.local") {
    return local(words);
  } else if (words[0] == "time.localdate") {

  } else if (words[0] == "time.list") {
    return list(words);
  } else if (words[0] == "time.string") {
    return stringtime(words);
  }
  throw Exception("Fatal implementation error in Time library in Time.cpp");
}

std::string Time::time(const std::vector<std::string> &vals) {
  if (vals.size() != 1) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
  time_t t;
  std::time(&t);
  return std::to_string(t);
}

std::string Time::date(const std::vector<std::string> &vals) {
  if (vals.size() != 1) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
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

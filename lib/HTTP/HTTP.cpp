/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: HTTP.cpp
 */

#include "HTTP.h"
#include "../../include/Exception.h"
#include "../../include/Interpreter.h"

HTTP::HTTP() {
  functions.insert(
      {"http.get", "http.post", "http.put", "http.head", "http.delete"});
}

std::set<std::string> HTTP::getFunctions() { return functions; }

std::string HTTP::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "http.get") {
    return get(words);
  } else if (words[0] == "http.post") {
    return post(words);
  } else if (words[0] == "http.put") {
    return put(words);
  } else if (words[0] == "http.head") {
    return head(words);
  } else if (words[0] == "http.delete") {
    return del(words);
  }
  throw Exception("Fatal implementation error in HTTP library in HTTP.cpp");
}

std::string HTTP::get(const std::vector<std::string> &vals) {
  return std::__cxx11::string();
}

std::string HTTP::head(const std::vector<std::string> &vals) {
  return std::__cxx11::string();
}

std::string HTTP::put(const std::vector<std::string> &vals) {
  return std::__cxx11::string();
}

std::string HTTP::post(const std::vector<std::string> &vals) {
  return std::__cxx11::string();
}

std::string HTTP::del(const std::vector<std::string> &vals) {
  return std::__cxx11::string();
}

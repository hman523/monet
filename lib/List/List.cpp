/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: List.cpp
 */

#include "List.h"
#include "../../include/Exception.h"
#include "../../include/Interpreter.h"

List::List() {
  functions.insert(
      {"list.nthtail", "list.get", "list.sort", "list.filter", "list.length"});
}

std::set<std::string> List::getFunctions() { return functions; }

std::string List::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "list.nthtail") {
    return nthTail(words);
  } else if (words[0] == "list.get") {
    return get(words);
  } else if (words[0] == "list.sort") {
    return sort(words);
  } else if (words[0] == "list.filter") {
    return filter(words);
  } else if (words[0] == "list.length") {
    return length(words);
  }
  throw Exception("Fatal implementation error in List::eval in List.cpp");
}

std::string List::evalstring(const std::string &val) {
  if (Interpreter::Instance()->isParens(val)) {
    return Interpreter::Instance()->eval(val);
  } else if (Interpreter::Instance()->isString(val)) {
    return Interpreter::Instance()->removequotes(val);
  } else {
    return Interpreter::Instance()->strtostr(val);
  }
}

std::string List::evallist(const std::string &val) {
  if (Interpreter::Instance()->isParens(val)) {
    return Interpreter::Instance()->eval(val);
  } else if (Interpreter::Instance()->isList(val)) {
    return val;
  } else {
    return Interpreter::Instance()->strtolist(val);
  }
}

std::string List::nthTail(const std::vector<std::string> &vals) {
  throw Exception("Fatal implementation error in List in List.cpp");
}

std::string List::get(const std::vector<std::string> &vals) {
  throw Exception("Fatal implementation error in List in List.cpp");
}

std::string List::sort(const std::vector<std::string> &vals) {
  throw Exception("Fatal implementation error in List in List.cpp");
}

std::string List::filter(const std::vector<std::string> &vals) {
  throw Exception("Fatal implementation error in List in List.cpp");
}

std::string List::length(const std::vector<std::string> &vals) {
  throw Exception("Fatal implementation error in List in List.cpp");
}

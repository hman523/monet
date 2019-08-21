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
  throw Exception("Fatal implementation error in File::eval in File.cpp");
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
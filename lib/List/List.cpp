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

List::List()
    : functions({"list.nthtail", "list.get", "list.sort", "list.filter",
                 "list.length"}) {}

std::set<std::string> List::getFunctions() { return functions; }

std::string List::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "list.nthtail") {
    return nthTail(words);
  } else if (words[0] == "list.get") {
    return get(words);
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

num List::evalnum(const std::string &val) {
  return Interpreter::Instance()->strToNum(val);
}

std::string List::head(const std::string &lst) {
  return Interpreter::Instance()->listSplit(lst).first;
}

std::string List::tail(const std::string &lst) {
  return Interpreter::Instance()->listSplit(lst).second;
}

bool List::isList(const std::string &lst) {
  return Interpreter::Instance()->memory.listexists(lst) ||
         Interpreter::Instance()->isList(lst);
}

bool List::isNum(const std::string &val) const {
  return Interpreter::Instance()->isNumber(val);
}

std::string List::nthTail(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("Wrong number of parameters for list.nthtail");
  }
  if (!isNum(vals[1])) {
    throw Exception("Wrong type in first parameter of list.nthtail");
  }
  if (!isList(vals[2])) {
    throw Exception("Wrong type in second parameter of list.nthtail");
  }
  throw Exception("nth Tail is not implemented yet");
  /**std::string lst = evallist(vals[2]);
  uint32_t n;
  std::string listhead = head(lst);
  std::string listtail = tail(lst);
  uint32_t index = 0;
  while (index < n) {
    listhead = head(listtail);
    listtail = tail(listtail);
  }
  return listtail;**/
}

std::string List::get(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("Wrong number of parameters for list.get");
  }
  if (!isList(vals[1])) {
    throw Exception("Wrong type in first parameter of list.get");
  }
  if (!isNum(vals[2])) {
    throw Exception("Wrong type in second parameter of list.get");
  }
  std::string lst = evallist(vals[1]);
  uint32_t n = 0;
  std::string listhead = head(lst);
  std::string listtail = tail(lst);
  num index = evalnum(vals[2]);
  while (index > n) {
    listhead = head(listtail);
    listtail = tail(listtail);
    ++n;
  }
  return listhead;
}

std::string List::length(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("Wrong number of parameters for list.length");
  }
  if (!isList(vals[1])) {
    throw Exception("Wrong type in first parameter of list.length");
  }
  if (!isNum(vals[2])) {
    throw Exception("Wrong type in second parameter of list.length");
  }
  throw Exception("Fatal implementation error in List in List.cpp");
}

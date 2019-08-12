/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Functional.cpp
 */

#include "Functional.h"
#include "../../include/Interpreter.h"
#include <vector>

Functional::Functional() : functions({"f.map", "f.apply", "f.reduce"}) {}

std::set<std::string> Functional::getFunctions() { return functions; }

std::string Functional::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "f.map") {
    return map(words);
  } else if (words[0] == "f.apply") {
    return apply(words);
  } else if (words[0] == "f.reduce") {
    return reduce(words);
  }
  throw Exception("A fatal implementation error has occurred in the Functional "
                  "library in Functional.cpp");
}

bool Functional::isFunction(const std::string &fn) {
  return Interpreter::Instance()->memory.isFunction(fn);
}

bool Functional::isList(const std::string &lst) {
  return Interpreter::Instance()->memory.listexists(lst) ||
         Interpreter::Instance()->isList(lst);
}

std::string Functional::head(const std::string &lst) {
  return Interpreter::Instance()->listSplit(lst).first;
}

std::string Functional::tail(const std::string &lst) {
  return Interpreter::Instance()->listSplit(lst).second;
}

std::string Functional::strToList(const std::string &lst) {
  return Interpreter::Instance()->strtolist(lst);
}

std::string Functional::map(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
  if (!isFunction(vals[1])) {
    throw Exception("First parameter in " + vals[0] + " is not a function");
  }
  if (!isList(vals[2])) {
    throw Exception("Second parameter in " + vals[0] + " is not a list");
  }
  std::string lst = strToList(vals[2]);
  std::string returnlst = "[";
  std::string listhead = head(lst);
  std::string listtail = tail(lst);
  while (listhead != "") {
    returnlst +=
        (Interpreter::Instance()->eval(vals[1] + " " + listhead) + " ");
    listhead = head(listtail);
    listtail = tail(listtail);
  }
  return returnlst.substr(0, returnlst.length() - 1) + "]";
}

std::string Functional::apply(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
}

std::string Functional::reduce(const std::vector<std::string> &vals) {
  if (vals.size() != 4) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
}

/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Math.cpp
 */

#include "Math.h"
#include "../../include/Exception.h"
#include "../../include/Interpreter.h"
#include <numeric>

namespace bm = boost::multiprecision;

Math::Math() { functions.insert({"math.exp"}); }

std::set<std::string> Math::getFunctions() { return functions; }

std::string Math::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "math.exp") {
    return exp(words);
  }
  throw Exception("Fatal implementation error in Math library in Math.cpp");
}

bool Math::isNum(const std::string &val) const {
  return Interpreter::Instance()->isNumber(val);
}

num Math::toNum(const std::string &val) const {
  return Interpreter::Instance()->strToNum(val);
}

std::string Math::numToStr(const num &val) const {
  return Interpreter::Instance()->normalizenumber(val);
}

std::string Math::exp(const std::vector<std::string> &vals) {
  if (vals.size() < 3) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
  bool allNums = std::all_of(vals.begin() + 1, vals.end(),
                             [&](std::string x) -> bool { return isNum(x); });
  if (!allNums) {
    throw Exception("Parameters not of type num in " + vals[0]);
  }
  std::vector<num> parameters;
  std::transform(vals.begin() + 1, vals.end(), std::back_inserter(parameters),
                 [&](std::string x) -> num { return toNum(x); });
  return "TODO MAKE EXP FUNCTION WORK";
}

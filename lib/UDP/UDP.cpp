/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: UDP.cpp
 */

#include "UDP.h"
#include "../../include/Interpreter.h"

UDP::UDP() : functions({"udp.send", "udp.receive"}) {}

std::set<std::string> UDP::getFunctions() { return functions; }

std::string UDP::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "udp.send") {
    send(words);
  } else if (words[0] == "udp.receive") {
    rec(words);
  }
  throw Exception("Fatal implementation error in UDP library in UDP.cpp");
}

std::string UDP::send(const std::vector<std::string> &expression) {}

std::string UDP::rec(const std::vector<std::string> &expression) {}

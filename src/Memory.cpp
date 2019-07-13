//
// Created by hunter on 6/29/19.
//

#include "Memory.h"
#include <cmath>
#include <iostream>
#include <sstream>

Memory::Memory() {
  functionnamespace.insert({"print",  "println", "quit", "boolean", "num",
                            "string", "read",    "add",  "sub",     "mul",
                            "div",    "and",     "or",   "nand",    "nor",
                            "xor",    "xnor",    "if",   "eq",      "ne",
                            "gt",     "lt",      "ge",   "le",      "define"});
  enterfn();
}

std::string Memory::get(std::string var) {
  if (varexists(var)) {
    if (getType(var) == "boolean") {
      if (getboolean(var)) {
        return "true";
      } else {
        return "false";
      }
    }
    if (getType(var) == "num") {
      if (fmod(getnum(var), 1) < .000001) {
        return std::to_string(((int)getnum(var)));
      } else {
        return std::to_string(getnum(var));
      }
    }
    if (getType(var) == "string") {
      return getstring(var);
    } else {
      std::cerr
          << "If you are seeing this message something is very, very wrong"
          << std::endl;
      return "";
    }
  } else {
    throw std::logic_error("Variable " + var + " not found");
  }
}

std::vector<std::string> Memory::getfn(std::string var) {
  if (!functioninuse(var)) {
    throw std::logic_error("Function " + var + " does not exist");
  }
  return functions[var];
}

std::string Memory::getstring(std::string var) {
  if (!strexists(var)) {
    throw std::logic_error("Variable " + var + " does not exist");
  }
  return strings.top()[var];
}

bool Memory::getboolean(std::string var) {
  if (!boolexists(var)) {
    throw std::logic_error("Variable " + var + " does not exist");
  }
  return booleans.top()[var];
}

double Memory::getnum(std::string var) {
  if (!numexists(var)) {
    throw std::logic_error("Variable " + var + " does not exist");
  }
  return nums.top()[var];
}

bool Memory::functioninuse(std::string val) {
  return functionnamespace.count(val) != 0;
}

void Memory::createfunction(std::string name, std::vector<std::string> code) {
  if (functioninuse(name)) {
    throw std::logic_error("Unable to redefine \"" + name + "\"");
  }
  functionnamespace.insert(name);
  functions.insert(
      std::pair<std::string, std::vector<std::string>>(name, code));
}

void Memory::createboolean(std::string name, bool value) {
  if (!varexists(name)) {
    booleans.top().insert(std::pair<std::string, bool>(name, value));
    variabletypes.top().insert(std::pair<std::string, std::string>(name, "boolean"));
  } else if (getType(name) == "boolean") {
    throw std::logic_error("Reinitialization of variable " + name);
  } else {
    throw std::logic_error("Variable " + name + " already initialized as a " +
                           getType(name));
  }
}

void Memory::createnum(std::string name, double num) {
  if (!varexists(name)) {
    nums.top().insert(std::pair<std::string, double>(name, num));
    variabletypes.top().insert(std::pair<std::string, std::string>(name, "num"));
  } else if (getType(name) == "num") {
    throw std::logic_error("Reinitialization of variable " + name);
  } else {
    throw std::logic_error("Variable " + name + " already initialized as a " +
                           getType(name));
  }
}

void Memory::createstring(std::string name, std::string str) {
  if (!varexists(name)) {
    strings.top().insert(std::pair<std::string, std::string>(name, str));
    variabletypes.top().insert(std::pair<std::string, std::string>(name, "string"));
  } else if (getType(name) == "string") {
    throw std::logic_error("Reinitialization of variable " + name);
  } else {
    throw std::logic_error("Variable " + name + " already initialized as a " +
                           getType(name));
  }
}

bool Memory::boolexists(std::string var) { return (booleans.top().count(var) != 0); }

bool Memory::strexists(std::string var) { return (strings.top().count(var) != 0); }

bool Memory::numexists(std::string var) { return (nums.top().count(var) != 0); }

bool Memory::varexists(std::string var) {
  return (variabletypes.top().count(var) != 0);
}

void Memory::enterfn() {
  variabletypes.push(std::map<std::string, std::string>());
  booleans.push(std::map<std::string, bool>());
  strings.push(std::map<std::string, std::string>());
  nums.push(std::map<std::string, double>());

}

void Memory::leavefn() {
  variabletypes.pop();
  booleans.pop();
  strings.pop();
  nums.pop();
}

std::string Memory::getType(std::string var) {
  return variabletypes.top()[var];
}

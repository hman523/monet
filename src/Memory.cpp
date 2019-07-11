//
// Created by hunter on 6/29/19.
//

#include "Memory.h"
#include <cmath>
#include <iostream>
#include <sstream>

Memory::Memory() {
  functionnamespace.insert({"print", "println", "quit", "boolean", "num",
                            "string", "read", "add", "sub", "mul", "div"});
}

std::string Memory::get(std::string var) {
  if (varexists(var)) {
    if (variabletypes[var] == "boolean") {
      if (booleans[var]) {
        return "true";
      } else {
        return "false";
      }
    }
    if (variabletypes[var] == "num") {
      if (fmod(nums[var], 1) < .000001) {
        return std::to_string(((int)nums[var]));
      } else {
        return std::to_string(nums[var]);
      }
    }
    if (variabletypes[var] == "string") {
      return strings[var];
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
  return strings[var];
}

bool Memory::getboolean(std::string var) {
  if (!boolexists(var)) {
    throw std::logic_error("Variable " + var + " does not exist");
  }
  return booleans[var];
}

double Memory::getnum(std::string var) {
  if (!numexists(var)) {
    throw std::logic_error("Variable " + var + " does not exist");
  }
  return nums[var];
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
    booleans.insert(std::pair<std::string, bool>(name, value));
    variabletypes.insert(std::pair<std::string, std::string>(name, "boolean"));
  } else if (variabletypes[name] == "boolean") {
    throw std::logic_error("Reinitialization of variable " + name);
  } else {
    throw std::logic_error("Variable " + name + " already initialized as a " +
                           variabletypes[name]);
  }
}

void Memory::createnum(std::string name, double num) {
  if (!varexists(name)) {
    nums.insert(std::pair<std::string, double>(name, num));
    variabletypes.insert(std::pair<std::string, std::string>(name, "num"));
  } else if (variabletypes[name] == "num") {
    throw std::logic_error("Reinitialization of variable " + name);
  } else {
    throw std::logic_error("Variable " + name + " already initialized as a " +
                           variabletypes[name]);
  }
}

void Memory::createstring(std::string name, std::string str) {
  if (!varexists(name)) {
    strings.insert(std::pair<std::string, std::string>(name, str));
    variabletypes.insert(std::pair<std::string, std::string>(name, "string"));
  } else if (variabletypes[name] == "string") {
    throw std::logic_error("Reinitialization of variable " + name);
  } else {
    throw std::logic_error("Variable " + name + " already initialized as a " +
                           variabletypes[name]);
  }
}

bool Memory::boolexists(std::string var) { return (booleans.count(var) != 0); }

bool Memory::strexists(std::string var) { return (strings.count(var) != 0); }

bool Memory::numexists(std::string var) { return (nums.count(var) != 0); }

bool Memory::varexists(std::string var) {
  return (variabletypes.count(var) != 0);
}

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
  if (variabletypes.count(var) != 0) {
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

std::string Memory::getstring(std::string var) {
  if (strings.count(var) == 0) {
    throw std::logic_error("Variable " + var + " does not exist");
  }
  return strings[var];
}

bool Memory::getboolean(std::string var) {
  if (booleans.count(var) == 0) {
    throw std::logic_error("Variable " + var + " does not exist");
  }
  return booleans[var];
}

double Memory::getnum(std::string var) {
  if (nums.count(var) == 0) {
    throw std::logic_error("Variable " + var + " does not exist");
  }
  return nums[var];
}

bool Memory::functioninuse(std::string val) {
  return functionnamespace.count(val) != 0;
}

void Memory::createboolean(std::string name, bool value) {
  if (variabletypes.count(name) == 0) {
    booleans[name] = value;
    variabletypes[name] = "boolean";
  } else if (variabletypes[name] == "boolean") {
    throw std::logic_error("Reinitialization of variable " + name);
  } else {
    throw std::logic_error("Variable " + name + " already initialized as a " +
                           variabletypes[name]);
  }
}

void Memory::createnum(std::string name, double num) {
  if (variabletypes.count(name) == 0) {
    nums[name] = num;
    variabletypes[name] = "num";
  } else if (variabletypes[name] == "num") {
    throw std::logic_error("Reinitialization of variable " + name);
  } else {
    throw std::logic_error("Variable " + name + " already initialized as a " +
                           variabletypes[name]);
  }
}

void Memory::createstring(std::string name, std::string str) {
  if (variabletypes.count(name) == 0) {
    strings[name] = str;
    variabletypes[name] = "string";
  } else if (variabletypes[name] == "string") {
    throw std::logic_error("Reinitialization of variable " + name);
  } else {
    throw std::logic_error("Variable " + name + " already initialized as a " +
                           variabletypes[name]);
  }
}

bool Memory::boolexists(std::string var) { return !(booleans.count(var) == 0); }

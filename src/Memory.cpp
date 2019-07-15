//
// Created by hunter on 6/29/19.
//

#include "Memory.h"
#include "Interpreter.h"
#include <cmath>
#include <iostream>
#include <sstream>

Memory::Memory() {
  functionnamespace.insert(
      {"print",  "println", "quit", "boolean",    "num",   "string",
       "read",   "add",     "sub",  "mul",        "div",   "and",
       "or",     "nand",    "nor",  "xor",        "xnor",  "if",
       "eq",     "ne",      "gt",   "lt",         "ge",    "le",
       "define", "return",  "end",  "subroutine", "defmem"});
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

std::vector<std::string> Memory::getsub(std::string var) {
  return subroutines[var];
}

std::vector<std::string> Memory::getmem(std::string var) { return mems[var]; }

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

num Memory::getnum(std::string var) {
  if (!numexists(var)) {
    throw std::logic_error("Variable " + var + " does not exist");
  }
  return nums.top()[var];
}

bool Memory::functioninuse(std::string val) {
  return isFunction(val) || isSubroutine(val) || isMem(val);
}

bool Memory::isFunction(std::string val) {
  return functionnamespace.count(val) != 0;
}

bool Memory::isSubroutine(std::string val) {
  return subroutines.count(val) != 0;
}

bool Memory::isMem(std::string val) { return mems.count(val) != 0; }

void Memory::createfunction(std::string name, std::vector<std::string> code) {
  if (functioninuse(name)) {
    throw std::logic_error("Unable to redefine \"" + name + "\"");
  }
  functionnamespace.insert(name);
  functions.insert(
      std::pair<std::string, std::vector<std::string>>(name, code));
}

void Memory::createsub(std::string name, std::vector<std::string> code) {
  if (functioninuse(name)) {
    throw std::logic_error("Unable to redefine \"" + name + "\"");
  }
  subroutinenamespace.insert(name);
  subroutines.insert(
      std::pair<std::string, std::vector<std::string>>(name, code));
}

void Memory::createmem(std::string name, std::vector<std::string> code) {
  if (functioninuse(name)) {
    throw std::logic_error("Unable to redefine \"" + name + "\"");
  }
  memnamespace.insert(name);
  mems.insert(std::pair<std::string, std::vector<std::string>>(name, code));
}

void Memory::createboolean(std::string name, bool value) {
  if (!varexists(name)) {
    booleans.top().insert(std::pair<std::string, bool>(name, value));
    variabletypes.top().insert(
        std::pair<std::string, std::string>(name, "boolean"));
  } else if (getType(name) == "boolean") {
    throw std::logic_error("Reinitialization of variable " + name);
  } else {
    throw std::logic_error("Variable " + name + " already initialized as a " +
                           getType(name));
  }
}

void Memory::createnum(std::string name, num number) {
  if (!varexists(name)) {
    nums.top().insert(std::pair<std::string, num>(name, number));
    variabletypes.top().insert(
        std::pair<std::string, std::string>(name, "num"));
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
    variabletypes.top().insert(
        std::pair<std::string, std::string>(name, "string"));
  } else if (getType(name) == "string") {
    throw std::logic_error("Reinitialization of variable " + name);
  } else {
    throw std::logic_error("Variable " + name + " already initialized as a " +
                           getType(name));
  }
}

bool Memory::boolexists(std::string var) {
  return (booleans.top().count(var) != 0);
}

bool Memory::strexists(std::string var) {
  return (strings.top().count(var) != 0);
}

bool Memory::numexists(std::string var) { return (nums.top().count(var) != 0); }

bool Memory::varexists(std::string var) {
  return (variabletypes.top().count(var) != 0);
}

void Memory::enterfn() {
  variabletypes.push(std::map<std::string, std::string>());
  booleans.push(std::map<std::string, bool>());
  nums.push(std::map<std::string, num>());
  strings.push(std::map<std::string, std::string>());
}

void Memory::enterfn(std::vector<std::string> vals,
                     std::vector<std::string> fndefinition) {

  std::map<std::string, std::string> nexttypes =
      std::map<std::string, std::string>();
  std::map<std::string, bool> nextbool = std::map<std::string, bool>();
  std::map<std::string, std::string> nextstring =
      std::map<std::string, std::string>();
  std::map<std::string, num> nextnum = std::map<std::string, num>();

  num numberOfParameters = (fndefinition.size() - 3) / 2.0;
  for (uint32_t x = 0; x < numberOfParameters; ++x) {
    std::string type = fndefinition.at((2 * x) + 3);
    std::string name = fndefinition.at((2 * x) + 4);
    std::string value = vals[x];
    if (type == "boolean") {
      nextbool.insert(std::pair<std::string, bool>(name, strtobool(value)));
      nexttypes.insert(std::pair<std::string, std::string>(name, "boolean"));
    } else if (type == "string") {
      nextstring.insert(
          std::pair<std::string, std::string>(name, strtostr(value)));
      nexttypes.insert(std::pair<std::string, std::string>(name, "string"));
    } else if (type == "num") {
      nextnum.insert(std::pair<std::string, num>(name, strtonum(value)));
      nexttypes.insert(std::pair<std::string, std::string>(name, "num"));
    } else if (type == "fn"){

    }
    else {
      std::cerr << "Type " << type << " does not exist" << std::endl;
    }
  }
  variabletypes.push(nexttypes);
  booleans.push(nextbool);
  nums.push(nextnum);
  strings.push(nextstring);
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

num Memory::strtonum(std::string number) {
  if (numexists(number)) {
    return getnum(number);
  }
  std::stringstream ss(number);
  num val;
  ss >> val;
  return val;
}

bool Memory::strtobool(std::string val) {
  bool isVariable = boolexists(val);
  auto isBoolean = [&](std::string value) -> bool {
    if (boolexists(value)) {
      return true;
    }
    return (value == "true" || value == "false" || value == "0" ||
            value == "1");
  };
  if ((!isVariable) && !isBoolean(val)) {
    std::cerr << "Calling strtobool on nonboolean value \"" << val << "\""
              << std::endl;
  }
  if (isVariable) {
    return getboolean(val);
  }
  return (val == "true" || val == "1");
}

std::string Memory::strtostr(std::string var) {
  if (strexists(var)) {
    return getstring(var);
  } else {
    return var;
  }
}

std::string *Memory::checkmem(std::string name, std::vector<std::string> call) {
  if (memvalues[name].count(call) == 0) {
    return nullptr;
  } else {
    return &memvalues[name][call];
  }
}

void Memory::insertmem(std::string name, std::vector<std::string> call,
                       std::string result) {
  memvalues[name][call] = result;
}

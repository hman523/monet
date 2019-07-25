/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Memory.cpp
 */

#include "Memory.h"
#include "Interpreter.h"
#include <cmath>
#include <iostream>
#include <sstream>

//Libraries
#include "../lib/File.h"


Memory::Memory() {
  reservedwords.insert(
      {"print",  "println", "quit", "boolean", "num",    "string", "read",
       "add",    "sub",     "mul",  "div",     "and",    "or",     "nand",
       "nor",    "xor",     "xnor", "if",      "eq",     "ne",     "gt",
       "lt",     "ge",      "le",   "define",  "return", "end",    "subroutine",
       "defmem", "load",    "list", "cons",    "head",   "tail",   "null"});
  libraries.insert({"file"});
  enterfn();
}

std::string Memory::get(const std::string &var) const {
  if (varexists(var)) {
    if (getType(var) == "boolean") {
      if (getboolean(var)) {
        return "true";
      } else {
        return "false";
      }
    } else if (getType(var) == "num") {
      if (fmod(getnum(var), 1) < .000001) {
        return std::to_string(((int)getnum(var)));
      } else {
        return std::to_string(getnum(var));
      }
    } else if (getType(var) == "string") {
      return getstring(var);
    } else if (getType(var) == "list") {
      return getlist(var);
    } else {
      std::cerr
          << "If you are seeing this message something is very, very wrong"
          << std::endl;
      return "";
    }

  } else {
    throw Exception("Variable " + var + " not found");
  }
}

std::vector<std::string> Memory::getfn(const std::string &var) const {
  if (!functioninuse(var)) {
    throw Exception("Cannot make call to " + var);
  } else if (!functionbindings.empty()) {
    if (functionbindings.top().count(var) != 0) {
      return getfn(functionbindings.top().at(var));
    }
  }
  if (isFunction(var)) {
    return functions.at(var);
  } else if (isSubroutine(var)) {
    return subroutines.at(var);
  } else if (isMem(var)) {
    return mems.at(var);
  } else {
    throw Exception("Fatal implementation error in the interpreter");
  }
  // The code will never reach this point, this is just to shush the compiler
  return std::vector<std::string>();
}

std::string Memory::getstring(const std::string &var) const {
  if (!strexists(var)) {
    throw Exception("Variable " + var + " does not exist");
  }
  return strings.top().at(var);
}

bool Memory::getboolean(const std::string &var) const {
  if (!boolexists(var)) {
    throw Exception("Variable " + var + " does not exist");
  }
  return booleans.top().at(var);
}

num Memory::getnum(const std::string &var) const {
  if (!numexists(var)) {
    throw Exception("Variable " + var + " does not exist");
  }
  return nums.top().at(var);
}

std::string Memory::getlist(const std::string &var) const {
  if (!listexists(var)) {
    throw Exception("Variable " + var + " does not exist");
  }
  return lists.top().at(var);
}

bool Memory::functioninuse(const std::string &val) const {
  bool inStack = false;
  if (!functionbindings.empty()) {
    inStack = functionbindings.top().count(val) != 0;
  }
  return isBuiltInFn(val) || isFunction(val) || isSubroutine(val) ||
         isMem(val) || inStack;
}

bool Memory::isBuiltInFn(const std::string &val) const {
  return reservedwords.count(val) != 0;
}

bool Memory::isFunction(const std::string &val) const {
  return functionnamespace.count(val) != 0;
}

bool Memory::isSubroutine(const std::string &val) const {
  return subroutinenamespace.count(val) != 0;
}

bool Memory::isMem(const std::string &val) const {
  return memnamespace.count(val) != 0;
}

void Memory::createfunction(const std::string &name,
                            const std::vector<std::string> &code) {
  if (functioninuse(name)) {
    throw Exception("Unable to redefine \"" + name + "\"");
  }
  functionnamespace.insert(name);
  functions.insert(
      std::pair<std::string, std::vector<std::string>>(name, code));
}

void Memory::createsub(const std::string &name,
                       const std::vector<std::string> &code) {
  if (functioninuse(name)) {
    throw Exception("Unable to redefine \"" + name + "\"");
  }
  subroutinenamespace.insert(name);
  subroutines.insert(
      std::pair<std::string, std::vector<std::string>>(name, code));
}

void Memory::createmem(const std::string &name,
                       const std::vector<std::string> &code) {
  if (functioninuse(name)) {
    throw Exception("Unable to redefine \"" + name + "\"");
  }
  memnamespace.insert(name);
  mems.insert(std::pair<std::string, std::vector<std::string>>(name, code));
}

void Memory::createboolean(const std::string &name, bool value) {
  if (!varexists(name)) {
    booleans.top().insert(std::pair<std::string, bool>(name, value));
    variabletypes.top().insert(
        std::pair<std::string, std::string>(name, "boolean"));
  } else if (getType(name) == "boolean") {
    throw Exception("Reinitialization of variable " + name);
  } else {
    throw Exception("Variable " + name + " already initialized as a " +
                    getType(name));
  }
}

void Memory::createnum(const std::string &name, num number) {
  if (!varexists(name)) {
    nums.top().insert(std::pair<std::string, num>(name, number));
    variabletypes.top().insert(
        std::pair<std::string, std::string>(name, "num"));
  } else if (getType(name) == "num") {
    throw Exception("Reinitialization of variable " + name);
  } else {
    throw Exception("Variable " + name + " already initialized as a " +
                    getType(name));
  }
}

void Memory::createstring(const std::string &name, const std::string &str) {
  if (!varexists(name)) {
    strings.top().insert(std::pair<std::string, std::string>(name, str));
    variabletypes.top().insert(
        std::pair<std::string, std::string>(name, "string"));
  } else if (getType(name) == "string") {
    throw Exception("Reinitialization of variable " + name);
  } else {
    throw Exception("Variable " + name + " already initialized as a " +
                    getType(name));
  }
}

void Memory::createlist(const std::string &name, const std::string &list) {
  if (!varexists(name)) {
    lists.top().insert(std::pair<std::string, std::string>(name, list));
    variabletypes.top().insert(
        std::pair<std::string, std::string>(name, "list"));
  } else if (getType(name) == "list") {
    throw Exception("Reinitialization of variable " + name);
  } else {
    throw Exception("Variable " + name + " already initialized as a " +
                    getType(name));
  }
}

bool Memory::boolexists(const std::string &var) const {
  return (booleans.top().count(var) != 0);
}

bool Memory::strexists(const std::string &var) const {
  return (strings.top().count(var) != 0);
}

bool Memory::numexists(const std::string &var) const {
  return (nums.top().count(var) != 0);
}

bool Memory::listexists(const std::string &var) const {
  return (lists.top().count(var) != 0);
}

bool Memory::varexists(const std::string &var) const {
  return (variabletypes.top().count(var) != 0);
}

void Memory::enterfn() {
  variabletypes.push(std::map<std::string, std::string>());
  booleans.push(std::map<std::string, bool>());
  nums.push(std::map<std::string, num>());
  strings.push(std::map<std::string, std::string>());
  lists.push(std::map<std::string, std::string>());
}

void Memory::enterfn(const std::vector<std::string> &vals,
                     const std::vector<std::string> &fndefinition) {

  std::map<std::string, std::string> nexttypes =
      std::map<std::string, std::string>();
  std::map<std::string, bool> nextbool = std::map<std::string, bool>();
  std::map<std::string, std::string> nextstring =
      std::map<std::string, std::string>();
  std::map<std::string, std::string> nextlist =
      std::map<std::string, std::string>();
  std::map<std::string, num> nextnum = std::map<std::string, num>();
  std::map<std::string, std::string> nextfns =
      std::map<std::string, std::string>();
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
    } else if (type == "list") {
      nextlist.insert(
          std::pair<std::string, std::string>(name, strtolist(value)));
      nexttypes.insert(std::pair<std::string, std::string>(name, "list"));
    } else if (type == "num") {
      nextnum.insert(std::pair<std::string, num>(name, strtonum(value)));
      nexttypes.insert(std::pair<std::string, std::string>(name, "num"));
    } else if (type == "fn") {
      nextfns.insert(std::pair<std::string, std::string>(name, value));
    } else {
      std::cerr << "Type " << type << " does not exist" << std::endl;
    }
  }
  variabletypes.push(nexttypes);
  booleans.push(nextbool);
  nums.push(nextnum);
  strings.push(nextstring);
  lists.push(nextlist);
  functionbindings.push(nextfns);
}

void Memory::leavefn() {
  // So much popping you would think someone is making popcorn
  // I understand if you stop reading the code after that comment
  variabletypes.pop();
  booleans.pop();
  strings.pop();
  nums.pop();
  lists.pop();
  functionbindings.pop();
}

std::string Memory::getType(const std::string &var) const {
  return variabletypes.top().at(var);
}

num Memory::strtonum(const std::string &number) const {
  if (numexists(number)) {
    return getnum(number);
  }
  std::stringstream ss(number);
  num val;
  ss >> val;
  return val;
}

bool Memory::strtobool(const std::string &val) const {
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

std::string Memory::strtostr(const std::string &var) const {
  if (strexists(var)) {
    return getstring(var);
  } else {
    return var;
  }
}

std::string Memory::strtolist(const std::string &val) const {
  return listexists(val) ? getlist(val) : val;
}

std::string *Memory::checkmem(const std::string &name,
                              const std::vector<std::string> &call) {
  if (memvalues[name].count(call) == 0) {
    return nullptr;
  } else {
    return &memvalues[name][call];
  }
}

void Memory::insertmem(const std::string &name,
                       const std::vector<std::string> &call,
                       const std::string &result) {
  memvalues[name][call] = result;
}

std::string Memory::getBinding(const std::string &var) const {
  return functionbindings.top().at(var);
}

void Memory::addLibrary(const std::string &libraryname) {
  if(libraries.count(libraryname) == 0){
    throw Exception("Cannot include library " + libraryname + " because it does not exist");
  }
  if(libraryname == "file"){
    File *file = new File();
    libraryInstances.insert(std::pair<std::string, File *>("file", file));
    reservedwords.insert(file->getFunctions().begin(), file->getFunctions().end());
  }
}

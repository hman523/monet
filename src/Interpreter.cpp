/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Interpreter.cpp
 */

#include "Interpreter.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <numeric>
#include <sstream>

const char ENDOFFUN = '\17';
const std::string REPLPROMPT = "> ";

/**
 * Default constructor
 * Used when using the REPL
 */
Interpreter::Interpreter() { repl(); }

/**
 * Constructor
 * @param filename - the file you want to interpret
 */
Interpreter::Interpreter(std::string filename) {
  code = loadCodeFromFile(filename);
  interpret();
}

/**
 * interpret function- essentially iterates over the code and runs eval on each
 * line
 */
void Interpreter::interpret() {
  std::for_each(code.begin(), code.end(),
                [&](std::string line) -> void { eval(line); });
}

void Interpreter::repl() {
  std::string function;
  bool inFunction = false;
  while (true) {
    std::cout << REPLPROMPT << std::flush;
    std::string input;
    std::getline(std::cin, input);
    if (input == "") {
      continue;
    }
    if (input.substr(0, FUNCTION_DECLARATION_NAME.length()) ==
            FUNCTION_DECLARATION_NAME ||
        input.substr(0, SUBROUTINE_DECLARATION_NAME.length()) ==
            SUBROUTINE_DECLARATION_NAME ||
        input.substr(0, MEM_DECLARATION_NAME.length()) ==
            MEM_DECLARATION_NAME) {
      inFunction = true;
    }
    if (input.substr(0, FUNCTION_END_NAME.length()) == FUNCTION_END_NAME) {
      inFunction = false;
      std::cout << eval(function) << std::endl;
      function = "";
      continue;
    }
    if (!inFunction) {
      std::cout << eval(input) << std::endl;
    } else {
      function += input + ENDOFFUN;
    }
  }
}

/**
 * load code from file
 * @param filename the file you want to load from
 * @return a vector of strings where each line is the code;
 */
std::vector<std::string>
Interpreter::loadCodeFromFile(const std::string &filename) {
  std::vector<std::string> loadedcode;
  std::ifstream infile;
  infile.open(filename);
  std::string line;
  std::string fn;
  bool inFunction = false;
  while (std::getline(infile, line)) {
    // Some logic to make it so function declarations are not split up
    if (line.substr(0, FUNCTION_DECLARATION_NAME.length()) ==
            FUNCTION_DECLARATION_NAME ||
        line.substr(0, SUBROUTINE_DECLARATION_NAME.length()) ==
            SUBROUTINE_DECLARATION_NAME ||
        line.substr(0, MEM_DECLARATION_NAME.length()) == MEM_DECLARATION_NAME) {
      inFunction = true;
    }
    if (line.substr(0, FUNCTION_END_NAME.length()) == FUNCTION_END_NAME) {
      inFunction = false;
      loadedcode.push_back(fn);
      fn = "";
      continue;
    }
    if (inFunction) {
      fn += line;
      // Use end of transmission character in between each line
      fn += ENDOFFUN;
    } else {
      if (line == "") {
        // Do nothing on empty lines
        continue;
      }
      loadedcode.push_back(line);
    }
  }
  return loadedcode;
}

/**
 * Eval function
 * @param value the line you want to evauluate
 * @return what the command evaluates to
 */
std::string Interpreter::eval(const std::string &value) {
  std::vector<std::string> words = split(value);
  if (words.size() == 0) {
    return "";
  } else if (words[0].length() > 1 && words[0].substr(0, 2) == "//") {
    // do nothing, this is a comment
    return "";
  } else if (isParens(words[0])) {
    return eval(removeparens(words[0]));
  } else if (memory.isBuiltInFn(words[0])) {
    return evalBuiltIns(value, words);
  } else if (isLibraryCall(words[0])) {

  } else if (memory.functioninuse(words[0])) {
    if (memory.isFunction(words[0])) {
      return call(words);
    } else if (memory.isSubroutine(words[0])) {
      return callsubroutine(words[0]);
    } else if (memory.isMem(words[0])) {
      return callmem(words);
    } else {
      // happens when we are nested in a function
      std::string fn = memory.getBinding(words[0]);
      if (memory.isBuiltInFn(fn)) {
        words[0] = fn;
        std::string s = "";
        for (uint32_t i = 0; i < words.size(); ++i) {
          s += words[i];
          if (i != words.size() - 1) {
            s += " ";
          }
        }
        return eval(s);
      } else if (memory.isFunction(fn)) {
        return call(words);
      } else if (memory.isSubroutine(fn)) {
        return callsubroutine(words[0]);
      } else if (memory.isMem(fn)) {
        return callmem(words);
      }
    }
  } else {
    throw Exception("Function \"" + words[0] + "\" does not exist");
  }
  return "";
}

std::string Interpreter::evalBuiltIns(const std::string &value,
                                      const std::vector<std::string> &words) {
  const char FIRSTLETTER = value.at(0);
  switch (FIRSTLETTER) {
  case 'a':
    if (words[0] == "add") {
      return normalizenumber(add(words));
    } else if (words[0] == "and") {
      return normalizebool(andfunc(words));
    }
    break;
  case 'b':
    if (words[0] == "boolean") {
      declareboolean(words);
      return "";
    }
    break;
  case 'c':
    if (words[0] == "cons") {
      return cons(words);
    }
    break;
  case 'd':
    if (words[0] == "define") {
      define(split(value, ENDOFFUN));
      return "";
    } else if (words[0] == "defmem") {
      defmem(split(value, ENDOFFUN));
      return "";
    } else if (words[0] == "div") {
      return normalizenumber(div(words));
    }
    break;
  case 'e':
    if (words[0] == "eq") {
      return normalizebool(comparison(words) == 0);
    }
    break;
  case 'f':
    break;
  case 'g':
    if (words[0] == "ge") {
      return normalizebool(comparison(words) >= 0);
    } else if (words[0] == "gt") {
      return normalizebool(comparison(words) > 0);
    }
    break;
  case 'h':
    if (words[0] == "head") {
      return head(words);
    }
    break;
  case 'i':
    if (words[0] == "if") {
      return ifstatement(words);
    } else if (words[0] == "import") {
      return import(words);
    }
    break;
  case 'j':
    break;
  case 'k':
    break;
  case 'l':
    if (words[0] == "le") {
      return normalizebool(comparison(words) <= 0);
    } else if (words[0] == "list") {
      declarelist(words);
      return "";
    } else if (words[0] == "load") {
      load(words);
      return "";
    } else if (words[0] == "lt") {
      return normalizebool(comparison(words) < 0);
    }
    break;
  case 'm':
    if (words[0] == "mul") {
      return normalizenumber(mul(words));
    }
    break;
  case 'n':
    if (words[0] == "nand") {
      return normalizebool(nandfunc(words));
    } else if (words[0] == "ne") {
      return normalizebool(comparison(words) != 0);
    } else if (words[0] == "nor") {
      return normalizebool(norfunc(words));
    } else if (words[0] == "not") {
      return normalizebool(notfunc(words));
    } else if (words[0] == "null") {
      return normalizebool(isNull(words));
    } else if (words[0] == "num") {
      declarenum(words);
      return "";
    }
    break;
  case 'o':
    if (words[0] == "or") {
      return normalizebool(orfunc(words));
    }
    break;
  case 'p':
    if (words[0] == "print") {
      print(words);
      return "";
    } else if (words[0] == "println") {
      println(words);
      return "";
    } else if (words[0] == "printall") {
      printcode();
      return "";
    }
    break;
  case 'q':
    if (words[0] == "quit") {
      quit(words);
      return "";
    }
    break;
  case 'r':
    if (words[0] == "read") {
      return read(words);
    }
    break;
  case 's':
    if (words[0] == "string") {
      declarestring(words);
      return "";
    } else if (words[0] == "sub") {
      return normalizenumber(sub(words));
    } else if (words[0] == "subroutine") {
      subroutine(split(value, ENDOFFUN));
      return "";
    }
    break;
  case 't':
    if (words[0] == "tail") {
      return tail(words);
    }
    break;
  case 'u':
    break;
  case 'v':
    break;
  case 'w':
    break;
  case 'x':
    if (words[0] == "xnor") {
      return normalizebool(xnorfunc(words));
    } else if (words[0] == "xor") {
      return normalizebool(xorfunc(words));
    }
    break;
  case 'y':
    break;
  case 'z':
    break;
  default:
    if (words[0] == "<=>") {
      return std::to_string(comparison(words));
    }
  }
  throw Exception("Fatal implementation error in evalBuiltIns. The standard "
                  "library is flawed.");
}

/**
 * printcode- a function that prints the code, really just for debugging
 */
void Interpreter::printcode() {
  std::copy(code.begin(), code.end(),
            std::ostream_iterator<std::string>(std::cout, "\n"));
}

/**
 * split function
 * @param str string you want to split
 * @param delim the delim char that is used to split words, default = ' '
 * @return a vector of strings, each is one statement (can be a string,
 * parenthesised statement, or just a value)
 */
std::vector<std::string> Interpreter::split(const std::string &str,
                                            char delim) const {
  std::vector<std::string> returnval;
  std::string temp = "";
  bool instr = false;
  int inparens = 0;
  int inlist = 0;
  for (uint32_t i = 0; i < str.length(); ++i) {
    if (str[i] == '(' && !instr) {
      ++inparens;
    }
    if (str[i] == ')' && !instr) {
      --inparens;
    }
    if (str[i] == '[' && !instr) {
      ++inlist;
    }
    if (str[i] == ']' && !instr) {
      --inlist;
    }
    if (str[i] == '"') {
      instr = !instr;
    }
    if (str[i] == delim && temp != "" && !instr && inparens == 0 &&
        inlist == 0) {
      returnval.push_back(temp);
      temp = "";
    } else if (inparens != 0 || inlist != 0) {
      temp += str[i];
    } else if (instr) {
      temp += str[i];
    } else {
      temp += tolower(str[i]);
    }
  }
  if (temp != "") {
    returnval.push_back(temp);
  }
  returnval.erase(std::remove_if(returnval.begin(), returnval.end(),
                                 [](std::string x) -> bool {
                                   return (x == "" || x == " ");
                                 }),
                  returnval.end());
  return returnval;
}

/**
 * list split function
 * @param list the list you want to split
 * @return a pair where first is the head, second is the tail
 */
std::pair<std::string, std::string>
Interpreter::listSplit(const std::string &list) const {
  if (!isList(list)) {
    throw Exception("Unable to parse a non list as a list");
  }
  std::string rawlist = removelist(strtolist(list));
  std::string head, tail;
  if (rawlist == "") {
    head = "";
    tail = " ";
  } else {
    size_t index = std::string::npos;
    int nestcount = 0;
    int parencount = 0;
    for (uint32_t i = 0; i < rawlist.length(); ++i) {
      if (rawlist.at(i) == '[') {
        ++nestcount;
      } else if (rawlist.at(i) == ']') {
        --nestcount;
      } else if (rawlist.at(i) == '(') {
        ++parencount;
      } else if (rawlist.at(i) == ')') {
        --parencount;
      } else if (rawlist.at(i) == ' ' && nestcount == 0 && parencount == 0) {
        index = i;
        break;
      }
    }
    if (index == std::string::npos) {
      head = rawlist;
      tail = "[]";
    } else {
      head = rawlist.substr(0, index);
      tail = "[" + rawlist.substr(index + 1) + "]";
    }
  }
  return std::pair<std::string, std::string>(head, tail);
}

/**
 * isString function
 * @param val you want to check
 * @return true if val is a string in code
 */
bool Interpreter::isString(const std::string &val) const {
  return (val.length() >= 2 && val[0] == '"' && val[val.size() - 1] == '"');
}

/**
 * isList function
 * @param val the value you are testing to be a list
 * @return if the thing passed is a list
 */
bool Interpreter::isList(const std::string &val) const {
  return (val.length() >= 2 && val[0] == '[' && val[val.size() - 1] == ']');
  ;
}

/**
 * strtoint
 * @param num you want to convert
 * @return an integer evaluated from the number, only used in quit
 */
int Interpreter::strToInt(const std::string &num) const {
  if (memory.numexists(num)) {
    return (int)memory.getnum(num);
  }
  std::stringstream ss(num);
  int val;
  ss >> val;
  return val;
}

/**
 * strtonum
 * @param num you want to convert
 * @return a num that was the string
 */
num Interpreter::strToNum(const std::string &val) const {
  if (memory.numexists(val)) {
    return memory.getnum(val);
  }
  std::stringstream ss(val);
  num value;
  ss >> value;
  return value;
}

/**
 * strtobool
 * @param bool you want to convert
 * @return a bool that was the string
 */
bool Interpreter::strtobool(const std::string &val) const {
  bool isVariable = memory.boolexists(val);
  if ((!isVariable) && !isBoolean(val)) {
    std::cerr << "Calling strtobool on nonboolean value \"" << val << "\""
              << std::endl;
  }
  if (isVariable) {
    return memory.getboolean(val);
  }
  return (val == "true" || val == "1");
}

/**
 * strtostr
 * @param string you want to convert
 * @return a string that was the string, checks variables
 */
std::string Interpreter::strtostr(const std::string &var) const {
  if (memory.strexists(var)) {
    return memory.getstring(var);
  } else if (isString(var)) {
    return removequotes(var);
  } else {
    return var;
  }
}

/**
 * strtostr
 * @param string you want to convert
 * @return a list that was the string, checks variables
 */
std::string Interpreter::strtolist(const std::string &val) const {
  return memory.listexists(val) ? memory.getlist(val) : val;
}

std::string Interpreter::normalize(const std::string &val) const {
  if (memory.varexists(val)) {
    std::string type = memory.getType(val);
    if (type == "num") {
      return normalizenumber(strToNum(val));
    } else if (type == "boolean") {
      return normalizebool(strtobool(val));
    } else if (type == "string") {
      return strtostr(val);
    } else if (type == "list") {
      return strtolist(val);
    } else {
      throw Exception("Fatal implementation error");
    }
  } else {
    if (isNumber(val)) {
      return normalizenumber(strToNum(val));
    } else if (isBoolean(val)) {
      return normalizebool(strtobool(val));
    }
  }
  return val;
}

std::string Interpreter::removequotes(const std::string &original) const {
  if (isString(original)) {
    return original.substr(1, original.length() - 2);
  } else {
    return original;
  }
}

bool Interpreter::isParens(const std::string &statement) const {
  return (statement.length() >= 2 && statement[0] == '(' &&
          statement[statement.size() - 1] == ')');
}

std::string Interpreter::removeparens(const std::string &original) const {
  if (isParens(original)) {
    return original.substr(1, original.length() - 2);
  } else {
    return original;
  }
}

std::string Interpreter::removelist(const std::string &original) const {
  if (isList(original)) {
    return original.substr(1, original.length() - 2);
  } else {
    return original;
  }
}

bool Interpreter::isNumber(const std::string &value) const {
  if (memory.numexists(value)) {
    return true;
  }
  if (value.length() == 0) {
    return false;
  }
  if (value[0] == '-' || isdigit(value[0])) {
    for (uint32_t x = 0; x < value.length(); x++) {
      if (!isdigit(value[x])) {
        return false;
      }
    }
    return true;
  } else {
    return false;
  }
}

bool Interpreter::isBoolean(const std::string &value) const {
  if (memory.boolexists(value)) {
    return true;
  }
  return (value == "true" || value == "false" || value == "0" || value == "1");
}

std::string Interpreter::normalizenumber(num x) const {
  std::stringstream ss;
  ss << x;
  return ss.str();
}

std::string Interpreter::normalizebool(bool x) const {
  return x ? "true" : "false";
}

std::vector<bool>
Interpreter::parameterstobool(const std::vector<std::string> &vals) {
  std::vector<bool> parameters;
  std::transform(vals.begin() + 1, vals.end(), std::back_inserter(parameters),
                 [&](std::string in) -> bool {
                   if (isParens(in)) {
                     return strtobool(eval(in));
                   } else {
                     if (!isBoolean(in)) {
                       return memory.getboolean(in);
                     } else {
                       return strtobool(in);
                     }
                   }
                 });
  return parameters;
}

std::vector<num>
Interpreter::parameterstonums(const std::vector<std::string> &vals) {
  std::vector<num> parameters;
  std::transform(vals.begin() + 1, vals.end(), std::back_inserter(parameters),
                 [&](std::string in) -> num {
                   if (isParens(in)) {
                     return strToNum(eval(in));
                   } else {
                     if (!isNumber(in)) {
                       return memory.getnum(in);
                     } else {
                       return strToNum(in);
                     }
                   }
                 });
  return parameters;
}

std::vector<std::string>
Interpreter::evalParameters(const std::vector<std::string> &vals) {
  std::vector<std::string> parameters;
  std::transform(vals.begin() + 1, vals.end(), std::back_inserter(parameters),
                 [&](std::string in) -> std::string {
                   if (isParens(in)) {
                     return eval(in);
                   } else {
                     if (memory.varexists(in)) {
                       return memory.get(in);
                     } else if (isString(in)) {
                       return removequotes(in);
                     } else {
                       return in;
                     }
                   }
                 });
  return parameters;
}

void Interpreter::print(const std::vector<std::string> &words) {
  if (words.size() < 2) {
    std::cout << std::endl;
  } else {
    std::vector<std::string> params = evalParameters(words);
    for (uint32_t j = 0; j < params.size(); ++j) {
      if (params[j] == "~") {
        std::cout << std::endl;
      } else {
        std::cout << params[j] << std::flush;
      }
    }
  }
}

void Interpreter::println(const std::vector<std::string> &words) {
  if (words.size() < 2) {
    std::cout << std::endl;
  } else {
    std::vector<std::string> params = evalParameters(words);
    for (uint32_t j = 0; j < params.size(); ++j) {
      if (params[j] == "~") {
        std::cout << std::endl;
      } else {
        std::cout << params[j] << std::flush;
      }
    }
  }
  std::cout << std::endl;
}

void Interpreter::quit(const std::vector<std::string> &words) {
  if (words.size() == 1) {
    exit(EXIT_SUCCESS);
  } else {
    if (isParens(words[1])) {
      exit(strToInt(eval(words[1])));
    } else {
      exit(strToInt(words[1]));
    }
  }
}

void Interpreter::declarestring(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("Wrong number of parameters for string initialization");
  }
  if (isParens(vals[2])) {
    memory.createstring(vals[1], eval(vals[2]));
  } else {
    memory.createstring(vals[1], removequotes(vals[2]));
  }
}

void Interpreter::declareboolean(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("Wrong number of parameters for boolean initialization");
  }
  bool a;
  if (isParens(vals[2])) {
    std::string temp = eval(vals[2]);
    a = (temp == "true" || temp == "t" || temp == "1") ? true : false;
  } else {
    a = (vals[2] == "true" || vals[2] == "t" || vals[2] == "1") ? true : false;
  }
  memory.createboolean(vals[1], a);
}

void Interpreter::declarenum(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("Wrong number of parameters for num initialization");
  }
  if (isParens(vals[2])) {
    memory.createnum(vals[1], strToNum(eval(vals[2])));
  } else {
    memory.createnum(vals[1], strToNum(vals[2]));
  }
}

void Interpreter::declarelist(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("Wrong number of parameters for list initialization");
  }
  if (isParens(vals[2])) {
    memory.createlist(vals[1], strtolist(eval(vals[2])));
  } else {
    memory.createlist(vals[1], strtolist(vals[2]));
  }
}

std::string Interpreter::read(const std::vector<std::string> &vals) {
  if (vals.size() > 2) {
    throw Exception("Wrong number of parameters for reading");
  }
  std::string input;
  std::cin >> input;
  if (vals.size() == 2) {
    memory.createstring(vals[1], input);
  }
  return input;
}

std::string Interpreter::ifstatement(const std::vector<std::string> &vals) {
  if (vals.size() != 4) {
    throw Exception("Wrong number of inputs for if statement");
  }
  bool expr;
  if (isParens(vals[1])) {
    expr = strtobool(eval(vals[1]));
  } else {
    if (!isBoolean(vals[1])) {
      throw Exception("First value must be a boolean value in if statement");
    }
    expr = strtobool(vals[1]);
  }
  uint8_t index = expr ? 2 : 3;
  if (isParens(vals[index])) {
    return eval(vals[index]);
  } else if (memory.varexists(vals[index])) {
    return memory.get(vals[index]);
  } else {
    return vals[index];
  }
}

void Interpreter::define(const std::vector<std::string> &vals) {
  std::vector<std::string> definition = split(vals[0]);
  if (definition.size() < 3 || definition.size() % 2 == 0) {
    throw Exception("Cannot define function \"" + definition[2] +
                    "\" due to wrong number of parameters");
  }
  memory.createfunction(definition[2], vals);
}

std::string Interpreter::call(const std::vector<std::string> &vals) {
  std::string returnval = "";
  std::string functionname = vals[0];
  std::vector<std::string> fncode = memory.getfn(functionname);
  std::vector<std::string> definition = split(fncode[0]);
  double numberOfParameters = (definition.size() - 3) / 2.0;
  if (vals.size() - 1 != (numberOfParameters)) {
    throw Exception("Wrong number of parameters for call to function " +
                    vals[0]);
  }
  auto params = evalParameters(vals);
  memory.enterfn(params, definition);
  std::string returnname = "return";
  for (auto curr = fncode.begin() + 1; curr != fncode.end(); ++curr) {

    if ((*curr).substr(0, returnname.length()) == "return") {
      std::vector<std::string> words = split(*curr);
      if (isParens(words[1])) {
        returnval = eval(words[1]);
      } else {
        returnval = words[1];
      }
      break;
    } else {
      eval(*curr);
    }
  }
  memory.leavefn();
  return returnval;
}

void Interpreter::subroutine(const std::vector<std::string> &vals) {
  std::vector<std::string> definition = split(vals[0]);
  if (definition.size() != 2) {
    throw Exception("Cannot define subroutine \"" + definition[1] +
                    "\" due to parameters trying to be defined");
  }
  memory.createsub(definition[1], vals);
}

std::string Interpreter::callsubroutine(const std::string &name) {
  std::vector<std::string> subr = memory.getfn(name);
  std::for_each(subr.begin() + 1, subr.end(),
                [&](std::string line) -> void { eval(line); });
  return "";
}

void Interpreter::defmem(const std::vector<std::string> &vals) {
  std::vector<std::string> definition = split(vals[0]);
  if (definition.size() < 3 || definition.size() % 2 == 0) {
    throw Exception("Cannot define function \"" + definition[2] +
                    "\" due to wrong number of parameters");
  }
  memory.createmem(definition[2], vals);
}

std::string Interpreter::callmem(const std::vector<std::string> &vals) {
  std::string returnval = "";
  std::string functionname = vals[0];
  std::vector<std::string> fncode = memory.getfn(functionname);
  std::vector<std::string> definition = split(fncode[0]);
  double numberOfParameters = (definition.size() - 3) / 2.0;
  if (vals.size() - 1 != (numberOfParameters)) {
    throw Exception("Wrong number of parameters for call to function " +
                    vals[0]);
  }
  auto params = evalParameters(vals);
  std::string *check = memory.checkmem(functionname, params);
  if (check != nullptr) {
    return *check;
  }
  memory.enterfn(params, definition);
  std::string returnname = "return";
  for (auto curr = fncode.begin() + 1; curr != fncode.end(); ++curr) {

    if ((*curr).substr(0, returnname.length()) == returnname) {
      std::vector<std::string> words = split(*curr);
      if (isParens(words[1])) {
        returnval = eval(words[1]);
      } else {
        returnval = words[1];
      }
      break;
    } else {
      eval(*curr);
    }
  }
  memory.leavefn();
  memory.insertmem(functionname, params, returnval);
  return returnval;
}

/**
 * load the file given as parameter vals[1]
 * @param vals
 */
void Interpreter::load(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Must have one parameter for load");
  }
  const std::string filename =
      (isString(vals[1]) ? removequotes(vals[1]) : strtostr(vals[1]));
  std::vector<std::string> loadedcode = loadCodeFromFile(filename);
  std::for_each(loadedcode.begin(), loadedcode.end(),
                [&](std::string line) -> void { eval(line); });
}

num Interpreter::add(const std::vector<std::string> &vals) {
  if (vals.size() < 2) {
    throw Exception("Too few inputs for add");
  }
  std::vector<num> parameters = parameterstonums(vals);
  return std::accumulate(parameters.begin(), parameters.end(), num(0));
}

num Interpreter::sub(const std::vector<std::string> &vals) {
  if (vals.size() < 2) {
    throw Exception("Too few inputs for sub");
  }
  std::vector<num> parameters = parameterstonums(vals);
  return parameters[0] + std::accumulate(parameters.begin() + 1,
                                         parameters.end(), num(0),
                                         std::minus<>{});
}

num Interpreter::mul(const std::vector<std::string> &vals) {
  if (vals.size() < 2) {
    throw Exception("Too few inputs for mul");
  }
  std::vector<num> parameters = parameterstonums(vals);
  return std::accumulate(parameters.begin(), parameters.end(), num(1),
                         std::multiplies<>{});
}

num Interpreter::div(const std::vector<std::string> &vals) {
  if (vals.size() < 2) {
    throw Exception("Too few inputs for div");
  }
  std::vector<num> parameters = parameterstonums(vals);
  num curr = parameters[0];
  for (uint32_t i = 1; i < parameters.size(); ++i) {
    if (parameters[i] != 0) {
      curr /= parameters[i];
    } else {
      throw Exception("Divide by zero error in div function");
    }
  }
  return curr;
}

bool Interpreter::andfunc(const std::vector<std::string> &vals) {
  std::vector<bool> params = parameterstobool(vals);
  if (params.size() < 2) {
    throw Exception("and function has too few parameters");
  }
  for (uint32_t i = 0; i < params.size(); ++i) {
    if (!params[i]) {
      return false;
    }
  }
  return true;
}

bool Interpreter::orfunc(const std::vector<std::string> &vals) {
  std::vector<bool> params = parameterstobool(vals);
  if (params.size() < 2) {
    throw Exception("or function has too few parameters");
  }
  for (uint32_t i = 0; i < params.size(); ++i) {
    if (params[i]) {
      return true;
    }
  }
  return false;
}

bool Interpreter::notfunc(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Not function can only take one parameter");
  }
  return !(strtobool(vals[1]));
}

bool Interpreter::nandfunc(const std::vector<std::string> &vals) {
  if (vals.size() < 3) {
    throw Exception("nand function has too few parameters");
  }
  return !andfunc(vals);
}

bool Interpreter::norfunc(const std::vector<std::string> &vals) {
  if (vals.size() < 3) {
    throw Exception("nor function has too few parameters");
  }
  return !orfunc(vals);
}

bool Interpreter::xorfunc(const std::vector<std::string> &vals) {
  std::vector<bool> params = parameterstobool(vals);
  if (params.size() != 2) {
    throw Exception("xor function must have two parameters");
  }
  return (params[0] != params[1]);
}

bool Interpreter::xnorfunc(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("xnor function must have two parameters");
  }
  return !xorfunc(vals);
}

int Interpreter::comparison(const std::vector<std::string> &vals) {
  // return 0 if eq, 1 if greater than, -1 if less than
  if (vals.size() != 3) {
    throw Exception("Comparision can only be between two values");
  }
  std::string vals1 = isParens(vals[1]) ? eval(vals[1]) : vals[1];
  std::string vals2 = isParens(vals[2]) ? eval(vals[2]) : vals[2];

  if (isBoolean(vals1) && isBoolean(vals2)) {
    bool a, b;
    a = strtobool(vals1);
    b = strtobool(vals2);
    if (a == b) {
      return 0;
    } else if (a && !b) {
      return 1;
    } else {
      return -1;
    }
  }
  if (isNumber(vals1) && isNumber(vals2)) {
    num x, y;
    x = strToNum(vals1);
    y = strToNum(vals2);
    if (x == y) {
      return 0;
    } else if (x > y) {
      return 1;
    } else {
      return -1;
    }
  }
  if (isString(vals1) && isString(vals2)) {
    std::string a, b;
    a = strtostr(vals1);
    b = strtostr(vals2);
    int returnval = a.compare(b);
    if (returnval == 0) {
      return 0;
    } else {
      if (returnval > 0) {
        return 1;
      } else {
        return -1;
      }
    }
  }
  throw Exception("Comparison not permitted between different types");
  return 0;
}

std::string Interpreter::head(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters for head");
  }
  std::string headval =
      isParens(vals[1]) ? getHead(eval(vals[1])) : getHead(vals[1]);
  return (isParens(headval)) ? eval(headval) : headval;
}

std::string Interpreter::getHead(const std::string &val) const {
  if (!isList(val) && !memory.listexists(val)) {
    throw Exception("Head called on a non list");
  } else {
    return listSplit(strtolist(val)).first;
  }
}

std::string Interpreter::tail(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters for tail");
  }
  if (isParens(vals[1])) {
    return getTail(eval(vals[1]));
  } else {
    return getTail(vals[1]);
  }
}

std::string Interpreter::getTail(const std::string &val) const {
  if (!isList(val) && !memory.listexists(val)) {
    throw Exception("Tail called on a non list");
  } else {
    std::string tail = listSplit(strtolist(val)).second;
    if (tail == " ") {
      throw Exception("Tail called on null list");
    } else {
      return tail;
    }
  }
}

std::string Interpreter::cons(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("Wrong number of parameters for cons");
  }
  std::string h = isParens(vals[1]) ? eval(vals[1]) : normalize(vals[1]);
  std::string t = isParens(vals[2]) ? eval(vals[2]) : strtolist(vals[2]);
  return getcons(h, t);
}

std::string Interpreter::getcons(const std::string &val,
                                 const std::string &list) const {

  return (list != "[]") ? std::string("[") + val + std::string(" ") +
                              removelist(list) + std::string("]")
                        : std::string("[") + val + std::string("]");
}

bool Interpreter::isNull(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters for null");
  }
  if (isParens(vals[1])) {
    return "" == removelist(eval(vals[1]));
  } else {
    return "" == removelist(strtolist(vals[1]));
  }
}

bool Interpreter::isLibraryCall(const std::string &vals) const {
  std::vector<std::string> libraryDotFunc = split(vals, '.');
  return (libraryDotFunc.size() == 2 &&
          memory.libraryExists(libraryDotFunc[0]));
}

std::string Interpreter::import(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters for include");
  }
  std::string lib = isParens(vals[1]) ? eval(vals[1]) : strtostr(vals[1]);
  if (memory.libraryExists(lib)) {
    throw Exception("Library " + lib + " does not exist");
  }
  if (memory.librayImported(lib)) {
    throw Exception("Library " + lib + " already imported");
  }
  includeLibrary(lib);
  return "";
}

void Interpreter::includeLibrary(const std::string &libraryName) {
  memory.importLibrary(libraryName);
}

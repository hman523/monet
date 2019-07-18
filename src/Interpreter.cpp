//
// Created by hunter on 6/29/19.
//

#include "Interpreter.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <numeric>
#include <random>
#include <sstream>

/**
 * Constructor
 * @param filename - the file you want to interpret
 */
Interpreter::Interpreter(std::string filename) {
  code = loadcodefromfile(filename);
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

/**
 * load code from file
 * @param filename the file you want to load from
 * @return a vector of strings where each line is the code;
 */
std::vector<std::string>
Interpreter::loadcodefromfile(const std::string &filename) {
  std::vector<std::string> loadedcode;
  const std::string functiondeclarationname = "define";
  const std::string functionendname = "end";
  const std::string subroutinedeclarationname = "subroutine";
  const std::string functionmemdeclarationname = "defmem";
  std::ifstream infile;
  infile.open(filename);
  std::string line;
  std::string fn;
  bool inFunction = false;
  while (std::getline(infile, line)) {
    // Some logic to make it so function declarations are not split up
    if (line.substr(0, functiondeclarationname.length()) ==
            functiondeclarationname ||
        line.substr(0, subroutinedeclarationname.length()) ==
            subroutinedeclarationname ||
        line.substr(0, functionmemdeclarationname.length()) ==
            functionmemdeclarationname) {
      inFunction = true;
    }
    if (line.substr(0, functionendname.length()) == functionendname) {
      inFunction = false;
      loadedcode.push_back(fn);
      fn = "";
      continue;
    }
    if (inFunction) {
      fn += line;
      // Use end of transmission character in between each line
      fn += '\17';
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
  } else if (words[0] == "define") {
    define(split(value, '\17'));
    return "";
  } else if (words[0] == "subroutine") {
    subroutine(split(value, '\17'));
    return "";
  } else if (words[0] == "defmem") {
    defmem(split(value, '\17'));
    return "";
  } else if (words[0] == "load") {
    load(words);
    return "";
  } else if (words[0] == "quit") {
    quit(words);
  } else if (words[0] == "print") {
    print(words);
  } else if (words[0] == "println") {
    println(words);
  } else if (words[0] == "string") {
    declarestring(words);
  } else if (words[0] == "num") {
    declarenum(words);
  } else if (words[0] == "boolean") {
    declareboolean(words);
  } else if (words[0] == "read") {
    return read(words);
  } else if (words[0] == "if") {
    return ifstatement(words);
  } else if (words[0] == "printall") {
    printcode();
  } else if (words[0] == "add") {
    return normalizenumber(add(words));
  } else if (words[0] == "sub") {
    return normalizenumber(sub(words));
  } else if (words[0] == "mul") {
    return normalizenumber(mul(words));
  } else if (words[0] == "div") {
    return normalizenumber(div(words));
  } else if (words[0] == "and") {
    return normalizebool(andfunc(words));
  } else if (words[0] == "or") {
    return normalizebool(orfunc(words));
  } else if (words[0] == "not") {
    return normalizebool(notfunc(words));
  } else if (words[0] == "nand") {
    return normalizebool(nandfunc(words));
  } else if (words[0] == "nor") {
    return normalizebool(norfunc(words));
  } else if (words[0] == "xor") {
    return normalizebool(xorfunc(words));
  } else if (words[0] == "xnor") {
    return normalizebool(xnorfunc(words));
  } else if (words[0] == "eq") {
    return normalizebool(comparison(words) == 0);
  } else if (words[0] == "ne") {
    return normalizebool(comparison(words) != 0);
  } else if (words[0] == "gt") {
    return normalizebool(comparison(words) > 0);
  } else if (words[0] == "ge") {
    return normalizebool(comparison(words) >= 0);
  } else if (words[0] == "lt") {
    return normalizebool(comparison(words) < 0);
  } else if (words[0] == "le") {
    return normalizebool(comparison(words) <= 0);
  } else if (words[0] == "<=>") {
    return std::to_string(comparison(words));
  } else if (memory.functioninuse(words[0])) {
    if (memory.isFunction(words[0])) {
      return call(words);
    } else if (memory.isSubroutine(words[0])) {
      return callsubroutine(words[0]);
    } else if (memory.isMem(words[0])) {
      return callmem(words);
    } else {
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
    throw std::logic_error("Function \"" + words[0] + "\" does not exist");
  }
  return "";
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
  for (uint32_t i = 0; i < str.length(); ++i) {
    if (str[i] == '(' && !instr) {
      ++inparens;
    }
    if (str[i] == ')' && !instr) {
      --inparens;
    }
    if (str[i] == '"') {
      instr = !instr;
    }
    if (str[i] == delim && temp != "" && !instr && inparens == 0) {
      returnval.push_back(temp);
      temp = "";
    } else if (inparens != 0) {
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
 * isString function
 * @param val you want to check
 * @return true if val is a string in code
 */
bool Interpreter::isString(const std::string &val) const {
  return (val.length() >= 2 && val[0] == '"' && val[val.size() - 1] == '"');
}

/**
 * strtoint
 * @param num you want to convert
 * @return an integer evaluated from the number, only used in quit
 */
int Interpreter::strtoint(const std::string &num) const {
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
num Interpreter::strtonum(const std::string &number) const {
  if (memory.numexists(number)) {
    return memory.getnum(number);
  }
  std::stringstream ss(number);
  num val;
  ss >> val;
  return val;
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
  } else {
    return var;
  }
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
  if (fmod(x, 1) < .000001) {
    return std::to_string(((int)x));
  } else {
    return std::to_string(x);
  }
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
                     return strtobool(eval(removeparens(in)));
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
                     return strtonum(eval(removeparens(in)));
                   } else {
                     if (!isNumber(in)) {
                       return memory.getnum(in);
                     } else {
                       return strtonum(in);
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
                     return eval(removeparens(in));
                   } else {
                     if (memory.varexists(in)) {
                       return memory.get(in);
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
    for (uint32_t i = 1; i < words.size(); ++i) {
      if (isNumber(words[i])) {
        std::cout << words[i] << std::endl;
      } else if (isString(words[i])) {
        std::cout << removequotes(words[i]) << std::flush;
      } else if (isParens(words[i])) {
        std::cout << eval(removeparens(words[i])) << std::flush;
      } else if (words[i] == "~") {
        std::cout << std::endl;
      } else {
        std::cout << memory.get(words[i]) << std::flush;
      }
    }
  }
}

void Interpreter::println(const std::vector<std::string> &words) {
  if (words.size() < 2) {
    // do nothing
  } else {
    for (uint32_t i = 1; i < words.size(); ++i) {
      if (isNumber(words[i])) {
        std::cout << words[i] << std::endl;
      } else if (isString(words[i])) {
        std::cout << removequotes(words[i]) << std::flush;
      } else if (isParens(words[i])) {
        std::cout << eval(removeparens(words[i])) << std::flush;
      } else if (words[i] == "~") {
        std::cout << std::endl;
      } else {
        std::cout << memory.get(words[i]) << std::flush;
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
      exit(strtoint(eval(words[1])));
    } else {
      exit(strtoint(words[1]));
    }
  }
}

void Interpreter::declarestring(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw std::logic_error(
        "Wrong number of parameters for string initialization");
  }
  if (isParens(vals[2])) {
    memory.createstring(vals[1], eval(removeparens(vals[2])));
  } else {
    memory.createstring(vals[1], removequotes(vals[2]));
  }
}

void Interpreter::declareboolean(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw std::logic_error(
        "Wrong number of parameters for boolean initialization");
  }
  bool a;
  if (isParens(vals[2])) {
    std::string temp = eval(removeparens(vals[2]));
    a = (temp == "true" || temp == "t" || temp == "1") ? true : false;
  } else {
    a = (vals[2] == "true" || vals[2] == "t" || vals[2] == "1") ? true : false;
  }
  memory.createboolean(vals[1], a);
}

void Interpreter::declarenum(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw std::logic_error("Wrong number of parameters for num initialization");
  }
  if (isParens(vals[2])) {
    memory.createnum(vals[1], strtonum(eval(removeparens(vals[2]))));
  } else {
    memory.createnum(vals[1], strtonum(vals[2]));
  }
}

std::string Interpreter::read(const std::vector<std::string> &vals) {
  if (vals.size() > 2) {
    throw std::logic_error("Wrong number of parameters for reading");
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
    throw std::logic_error("Wrong number of inputs for if statement");
  }
  bool expr;
  if (isParens(vals[1])) {
    expr = strtobool(eval(removeparens(vals[1])));
  } else {
    if (!isBoolean(vals[1])) {
      throw std::logic_error(
          "First value must be a boolean value in if statement");
    }
    expr = strtobool(vals[1]);
  }
  uint8_t index = expr ? 2 : 3;
  if (isParens(vals[index])) {
    return eval(removeparens(vals[index]));
  } else if (memory.varexists(vals[index])) {
    return memory.get(vals[index]);
  } else {
    return vals[index];
  }
}

void Interpreter::define(const std::vector<std::string> &vals) {
  std::vector<std::string> definition = split(vals[0]);
  if (definition.size() < 3 || definition.size() % 2 == 0) {
    throw std::logic_error("Cannot define function \"" + definition[2] +
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
    throw std::logic_error("Wrong number of parameters for call to function " +
                           vals[0]);
  }
  auto params = evalParameters(vals);
  memory.enterfn(params, definition);
  std::string returnname = "return";
  for (auto curr = fncode.begin() + 1; curr != fncode.end(); ++curr) {

    if ((*curr).substr(0, returnname.length()) == "return") {
      std::vector<std::string> words = split(*curr);
      if (isParens(words[1])) {
        returnval = eval(removeparens(words[1]));
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
    throw std::logic_error("Cannot define subroutine \"" + definition[1] +
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
    throw std::logic_error("Cannot define function \"" + definition[2] +
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
    throw std::logic_error("Wrong number of parameters for call to function " +
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
        returnval = eval(removeparens(words[1]));
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
    throw std::logic_error("Must have two parameters for load");
  }
  const std::string filename =
      (isString(vals[1]) ? removequotes(vals[1]) : strtostr(vals[1]));
  std::vector<std::string> loadedcode = loadcodefromfile(filename);
  std::for_each(loadedcode.begin(), loadedcode.end(),
                [&](std::string line) -> void { eval(line); });
}

num Interpreter::add(const std::vector<std::string> &vals) {
  if (vals.size() < 2) {
    throw std::logic_error("Too few inputs for add");
  }
  std::vector<num> parameters = parameterstonums(vals);
  return std::accumulate(parameters.begin(), parameters.end(), 0);
}

num Interpreter::sub(const std::vector<std::string> &vals) {
  if (vals.size() < 2) {
    throw std::logic_error("Too few inputs for sub");
  }
  std::vector<num> parameters = parameterstonums(vals);
  return parameters[0] + std::accumulate(parameters.begin() + 1,
                                         parameters.end(), 0, std::minus<>{});
}

num Interpreter::mul(const std::vector<std::string> &vals) {
  if (vals.size() < 2) {
    throw std::logic_error("Too few inputs for mul");
  }
  std::vector<num> parameters = parameterstonums(vals);
  return std::accumulate(parameters.begin(), parameters.end(), 1,
                         std::multiplies<>{});
}

num Interpreter::div(const std::vector<std::string> &vals) {
  if (vals.size() < 2) {
    throw std::logic_error("Too few inputs for div");
  }
  std::vector<num> parameters = parameterstonums(vals);
  num curr = parameters[0];
  for (uint32_t i = 1; i < parameters.size(); ++i) {
    curr /= parameters[i];
  }
  return curr;
}

bool Interpreter::andfunc(const std::vector<std::string> &vals) {
  std::vector<bool> params = parameterstobool(vals);
  if (params.size() < 2) {
    throw std::logic_error("and function has too few parameters");
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
    throw std::logic_error("or function has too few parameters");
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
    throw std::logic_error("Not function can only take one parameter");
  }
  return !(strtobool(vals[1]));
}

bool Interpreter::nandfunc(const std::vector<std::string> &vals) {
  if (vals.size() < 3) {
    throw std::logic_error("nand function has too few parameters");
  }
  return !andfunc(vals);
}

bool Interpreter::norfunc(const std::vector<std::string> &vals) {
  if (vals.size() < 3) {
    throw std::logic_error("nor function has too few parameters");
  }
  return !orfunc(vals);
}

bool Interpreter::xorfunc(const std::vector<std::string> &vals) {
  std::vector<bool> params = parameterstobool(vals);
  if (params.size() != 2) {
    throw std::logic_error("xor function must have two parameters");
  }
  return (params[0] != params[1]);
}

bool Interpreter::xnorfunc(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw std::logic_error("xnor function must have two parameters");
  }
  return !xorfunc(vals);
}

int Interpreter::comparison(const std::vector<std::string> &vals) {
  // return 0 if eq, 1 if greater than, -1 if less than
  if (vals.size() != 3) {
    throw std::logic_error("Comparision can only be between two values");
  }
  std::string vals1 = isParens(vals[1]) ? eval(removeparens(vals[1])) : vals[1];
  std::string vals2 = isParens(vals[2]) ? eval(removeparens(vals[2])) : vals[2];

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
    x = strtonum(vals1);
    y = strtonum(vals2);
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
  throw std::logic_error("Comparison not permitted between different types");
  return 0;
}
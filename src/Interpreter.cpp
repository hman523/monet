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

Interpreter::Interpreter(std::string filename) {
  std::string functiondeclarationname = "define";
  std::string functionendname = "endfn";
  std::ifstream infile;
  infile.open(filename);
  std::string line;
  std::string fn;
  bool inFunction = false;
  while (std::getline(infile, line)) {

    if (line.substr(0, functiondeclarationname.length()) ==
        functiondeclarationname) {
      inFunction = true;
    }
    if (line.substr(0, functionendname.length()) == functionendname) {
      inFunction = false;
      code.push_back(fn);
      fn = "";
      continue;
    }
    if (inFunction) {
      fn += line;
      fn += "\17";
    } else {
      code.push_back(line);
    }
  }
  std::cout << "HI" << std::endl;

  interpret();
}

void Interpreter::interpret() {
  std::for_each(code.begin(), code.end(),
                [&](std::string line) -> void { eval(line); });
}

std::string Interpreter::eval(std::string value) {
  std::vector<std::string> words = split(value);
  if (words.size() == 0) {
    return "";
  } else if (words[0].length() > 1 && words[0].substr(0, 2) == "//") {
    // do nothing, this is a comment
    return "";
  } else if (words[0] == "define") {
    define(split(value, '\17'));
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
    return call(words);
  } else {
    throw std::logic_error("Function \"" + words[0] + "\" does not exist");
  }
  return "";
}

void Interpreter::printcode() {
  std::copy(code.begin(), code.end(),
            std::ostream_iterator<std::string>(std::cout, "\n"));
}

std::vector<std::string> Interpreter::split(std::string str, char delim) {
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
  return returnval;
}

bool Interpreter::isString(std::string val) {
  return (val.length() >= 2 && val[0] == '"' && val[val.size() - 1] == '"');
}

int Interpreter::strtoint(std::string num) {
  std::stringstream ss(num);
  int val;
  ss >> val;
  return val;
}

double Interpreter::strtonum(std::string num) {
  if (memory.numexists(num)) {
    return memory.getnum(num);
  }
  std::stringstream ss(num);
  double val;
  ss >> val;
  return val;
}

bool Interpreter::strtobool(std::string val) {
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

std::string Interpreter::strtostr(std::string var) {
  if (memory.strexists(var)) {
    return memory.getstring(var);
  } else {
    return var;
  }
}

std::string Interpreter::removequotes(std::string original) {
  if (isString(original)) {
    return original.substr(1, original.length() - 2);
  } else {
    return original;
  }
}

bool Interpreter::isParens(std::string statement) {
  return (statement.length() >= 2 && statement[0] == '(' &&
          statement[statement.size() - 1] == ')');
}

std::string Interpreter::removeparens(std::string original) {
  if (isParens(original)) {
    return original.substr(1, original.length() - 2);
  } else {
    return original;
  }
}

bool Interpreter::isNumber(std::string value) {
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

bool Interpreter::isBoolean(std::string value) {
  if (memory.boolexists(value)) {
    return true;
  }
  return (value == "true" || value == "false" || value == "0" || value == "1");
}

std::string Interpreter::normalizenumber(double x) {
  if (fmod(x, 1) < .000001) {
    return std::to_string(((int)x));
  } else {
    return std::to_string(x);
  }
}

std::string Interpreter::normalizebool(bool x) { return x ? "true" : "false"; }

std::vector<bool> Interpreter::parameterstobool(std::vector<std::string> vals) {
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

std::vector<double>
Interpreter::parameterstonums(std::vector<std::string> vals) {
  std::vector<double> parameters;
  std::transform(vals.begin() + 1, vals.end(), std::back_inserter(parameters),
                 [&](std::string in) -> double {
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

void Interpreter::print(std::vector<std::string> words) {
  if (words.size() < 2) {
    std::cout << std::endl;
  } else {
    for (uint32_t i = 1; i < words.size(); ++i) {
      if (isString(words[i])) {
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

void Interpreter::println(std::vector<std::string> words) {
  if (words.size() < 2) {
    // do nothing
  } else {
    for (uint32_t i = 1; i < words.size(); ++i) {
      if (isString(words[i])) {
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

void Interpreter::quit(std::vector<std::string> words) {
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

void Interpreter::declarestring(std::vector<std::string> vals) {
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

void Interpreter::declareboolean(std::vector<std::string> vals) {
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

void Interpreter::declarenum(std::vector<std::string> vals) {
  if (vals.size() != 3) {
    throw std::logic_error("Wrong number of parameters for num initialization");
  }
  if (isParens(vals[2])) {
    memory.createnum(vals[1], strtonum(eval(removeparens(vals[2]))));
  } else {
    memory.createnum(vals[1], strtonum(vals[2]));
  }
}

std::string Interpreter::read(std::vector<std::string> vals) {
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

std::string Interpreter::ifstatement(std::vector<std::string> vals) {
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
  } else {
    return vals[index];
  }
}

void Interpreter::define(std::vector<std::string> vals) {
  memory.createfunction(split(vals[0])[2], vals);
}

std::string Interpreter::call(std::vector<std::string> vals) {
  std::string functionname = vals[0];
  auto fncode = memory.getfn(functionname);
  // todo: figure out how to pass parameters, or rather call functions without
  // them using the global namespace
  return "";
}

double Interpreter::add(std::vector<std::string> vals) {
  if (vals.size() < 2) {
    throw std::logic_error("Too few inputs for add");
  }
  std::vector<double> parameters = parameterstonums(vals);
  return std::accumulate(parameters.begin(), parameters.end(), 0);
}

double Interpreter::sub(std::vector<std::string> vals) {
  if (vals.size() < 2) {
    throw std::logic_error("Too few inputs for sub");
  }
  std::vector<double> parameters = parameterstonums(vals);
  return parameters[0] + std::accumulate(parameters.begin() + 1,
                                         parameters.end(), 0, std::minus<>{});
}

double Interpreter::mul(std::vector<std::string> vals) {
  if (vals.size() < 2) {
    throw std::logic_error("Too few inputs for mul");
  }
  std::vector<double> parameters = parameterstonums(vals);
  return std::accumulate(parameters.begin(), parameters.end(), 1,
                         std::multiplies<>{});
}

double Interpreter::div(std::vector<std::string> vals) {
  if (vals.size() < 2) {
    throw std::logic_error("Too few inputs for div");
  }
  std::vector<double> parameters = parameterstonums(vals);
  double curr = parameters[0];
  for (uint32_t i = 1; i < parameters.size(); ++i) {
    curr /= parameters[i];
  }
  return curr;
}

bool Interpreter::andfunc(std::vector<std::string> vals) {
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

bool Interpreter::orfunc(std::vector<std::string> vals) {
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

bool Interpreter::notfunc(std::vector<std::string> vals) {
  if (vals.size() != 2) {
    throw std::logic_error("Not function can only take one parameter");
  }
  return !(strtobool(vals[1]));
}

bool Interpreter::nandfunc(std::vector<std::string> vals) {
  if (vals.size() < 3) {
    throw std::logic_error("nand function has too few parameters");
  }
  return !andfunc(vals);
}

bool Interpreter::norfunc(std::vector<std::string> vals) {
  if (vals.size() < 3) {
    throw std::logic_error("nor function has too few parameters");
  }
  return !orfunc(vals);
}

bool Interpreter::xorfunc(std::vector<std::string> vals) {
  std::vector<bool> params = parameterstobool(vals);
  if (params.size() != 2) {
    throw std::logic_error("xor function must have two parameters");
  }
  return (params[0] != params[1]);
}

bool Interpreter::xnorfunc(std::vector<std::string> vals) {
  if (vals.size() != 3) {
    throw std::logic_error("xnor function must have two parameters");
  }
  return !xorfunc(vals);
}

int Interpreter::comparison(std::vector<std::string> vals) {
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
    double x, y;
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

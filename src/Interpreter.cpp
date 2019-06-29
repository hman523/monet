//
// Created by hunter on 6/29/19.
//

#include "Interpreter.h"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>

Interpreter::Interpreter(std::string filename) {
  std::ifstream infile;
  infile.open(filename);
  std::string line;
  while (std::getline(infile, line)) {
    code.push_back(line);
  }
  interpret();
}

void Interpreter::interpret() {
  std::for_each(code.begin(), code.end(),
                [&](std::string line) -> void { eval(line); });
}

void Interpreter::eval(std::string value) {
  std::vector<std::string> words = split(value);
  if (words.size() == 0) {
    return;
  }
  if (words[0] == "quit") {
    quit(words);
  }
  if (words[0] == "print") {
    print(words);
  }
  if (words[0] == "println") {
    println(words);
  }
  if (words[0] == "string") {
    declarestring(words);
  }
  if (words[0] == "num") {
    declarenum(words);
  }
  if (words[0] == "boolean") {
    declareboolean(words);
  }
}

void Interpreter::printcode() {
  std::copy(code.begin(), code.end(),
            std::ostream_iterator<std::string>(std::cout, "\n"));
}

std::vector<std::string> Interpreter::split(std::string str, char delim) {
  std::vector<std::string> returnval;
  std::string temp = "";
  bool instr = false;
  for (uint32_t i = 0; i < str.length(); ++i) {
    if (str[i] == '"') {
      instr = !instr;
    }
    if (str[i] == delim && temp != "" && !instr) {
      returnval.push_back(temp);
      temp = "";
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
  std::stringstream ss(num);
  double val;
  ss >> val;
  return val;
}

std::string Interpreter::removequotes(std::string original) {
  if (isString(original)) {
    return original.substr(1, original.length() - 2);
  } else {
    return original;
  }
}

void Interpreter::print(std::vector<std::string> words) {
  if (words.size() < 2) {
    std::cout << std::endl;
  } else {
    for (uint32_t i = 1; i < words.size(); ++i) {
      if (isString(words[i])) {
        std::cout << removequotes(words[i]) << std::flush;
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
    exit(strtoint(words[1]));
  }
}

void Interpreter::declarestring(std::vector<std::string> vals) {
  if (vals.size() != 3) {
    throw std::logic_error(
        "Wrong number of parameters for string initialization");
  }
  memory.createstring(vals[1], removequotes(vals[2]));
}

void Interpreter::declareboolean(std::vector<std::string> vals) {
  if (vals.size() != 3) {
    throw std::logic_error(
        "Wrong number of parameters for boolean initialization");
  }
  bool a =
      (vals[2] == "true" || vals[2] == "t" || vals[2] == "1") ? true : false;
  memory.createboolean(vals[1], a);
}

void Interpreter::declarenum(std::vector<std::string> vals) {
  if (vals.size() != 3) {
    throw std::logic_error("Wrong number of parameters for num initialization");
  }
  memory.createnum(vals[1], strtonum(vals[2]));
}

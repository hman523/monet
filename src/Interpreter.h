//
// Created by hunter on 6/29/19.
//

#ifndef BIPL_INTERPRETER_H
#define BIPL_INTERPRETER_H

#include "Memory.h"
#include <iostream>
#include <map>
#include <set>
#include <vector>

typedef double num;

class Interpreter {
public:
  Interpreter() = delete;
  Interpreter(std::string filename);

private:
  // helper functions
  void interpret();
  std::string eval(const std::string &value);
  std::vector<std::string> split(const std::string &str, const char delim = ' ');
  bool isString(const std::string &val);
  int strtoint(const std::string &num);
  num strtonum(const std::string &num);
  bool strtobool(const std::string &val);
  std::string strtostr(const std::string &str);
  std::string removequotes(const std::string &original);
  bool isParens(const std::string &statement);
  std::string removeparens(const std::string &original);
  bool isNumber(const std::string &value);
  bool isBoolean(const std::string &value);
  std::string normalizenumber(const num &x);
  std::string normalizebool(const bool &x);
  std::vector<bool> parameterstobool(const std::vector<std::string> &vals);
  std::vector<num> parameterstonums(const std::vector<std::string> &vals);
  std::vector<std::string> evalParameters(const std::vector<std::string> &vals);

  // command functions for the interpreting
  void printcode();
  void quit(const std::vector<std::string> &vals);
  void print(const std::vector<std::string> &vals);
  void println(const std::vector<std::string> &vals);
  void declarestring(const std::vector<std::string> &vals);
  void declareboolean(const std::vector<std::string> &vals);
  void declarenum(const std::vector<std::string> &vals);
  std::string read(const std::vector<std::string> &vals);
  std::string ifstatement(const std::vector<std::string> &vals);

  // function functions
  void define(const std::vector<std::string> &vals);
  std::string call(const std::vector<std::string> &vals);

  // Math functions
  num add(const std::vector<std::string> &vals);
  num sub(const std::vector<std::string> &vals);
  num mul(const std::vector<std::string> &vals);
  num div(const std::vector<std::string> &vals);

  // logic functions
  bool andfunc(const std::vector<std::string> &vals);
  bool orfunc(const std::vector<std::string> &vals);
  bool notfunc(const std::vector<std::string> &vals);
  bool nandfunc(const std::vector<std::string> &vals);
  bool norfunc(const std::vector<std::string> &vals);
  bool xorfunc(const std::vector<std::string> &vals);
  bool xnorfunc(const std::vector<std::string> &vals);

  // Equality functions
  int comparison(const std::vector<std::string> &vals);

  // memory
  std::vector<std::string> code;
  Memory memory;
};

#endif // BIPL_INTERPRETER_H

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

class Interpreter {
public:
  Interpreter() = delete;
  Interpreter(std::string filename);

private:
  // helper functions
  void interpret();
  std::string eval(std::string value);
  std::vector<std::string> split(std::string str, char delim = ' ');
  bool isString(std::string val);
  int strtoint(std::string num);
  double strtonum(std::string num);
  std::string removequotes(std::string original);
  bool isParens(std::string statement);
  std::string removeparens(std::string original);
  bool isNumber(std::string value);
  std::string normalizenumber(double x);

  // command functions for the interpretting
  void printcode();
  void quit(std::vector<std::string> vals);
  void print(std::vector<std::string> vals);
  void println(std::vector<std::string> vals);
  void declarestring(std::vector<std::string> vals);
  void declareboolean(std::vector<std::string> vals);
  void declarenum(std::vector<std::string> vals);
  void read(std::vector<std::string> vals);

  // Math functions
  double add(std::vector<std::string> vals);
  double sub(std::vector<std::string> vals);
  double mul(std::vector<std::string> vals);

  double div(std::vector<std::string> vals);

  // memory
  std::vector<std::string> code;
  Memory memory;
};

#endif // BIPL_INTERPRETER_H

//
// Created by hunter on 6/29/19.
//

#ifndef MONET_INTERPRETER_H
#define MONET_INTERPRETER_H

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
  std::vector<std::string> loadcodefromfile(const std::string &filename);
  std::string eval(const std::string &value);
  std::vector<std::string> split(const std::string &str,
                                 char delim = ' ') const;
  bool isString(const std::string &val) const;
  bool isList(const std::string &val) const;

  int strtoint(const std::string &num) const;
  num strtonum(const std::string &num) const;
  bool strtobool(const std::string &val) const;
  std::string strtostr(const std::string &str) const;
  std::string strtolist(const std::string &val) const;
  std::string removequotes(const std::string &original) const;

  bool isParens(const std::string &statement) const;
  std::string removeparens(const std::string &original) const;
  std::string removelist(const std::string &original) const;
  bool isNumber(const std::string &value) const;
  bool isBoolean(const std::string &value) const;

  std::string normalizenumber(num x) const;
  std::string normalizebool(bool x) const;
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
  void declarelist(const std::vector<std::string> &vals);
  std::string read(const std::vector<std::string> &vals);
  std::string ifstatement(const std::vector<std::string> &vals);

  // function functions
  void define(const std::vector<std::string> &vals);
  std::string call(const std::vector<std::string> &vals);
  void subroutine(const std::vector<std::string> &vals);
  std::string callsubroutine(const std::string &name);
  void defmem(const std::vector<std::string> &vals);
  std::string callmem(const std::vector<std::string> &vals);
  void load(const std::vector<std::string> &vals);

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

  // List functions
  std::string head(const std::vector<std::string> &vals);
  std::string getHead(const std::string &val) const;
  std::string tail(const std::vector<std::string> &vals);
  std::string getTail(const std::string &val) const;
  std::string cons(const std::vector<std::string> &vals);
  std::string getcons(const std::string &val, const std::string &list) const;
  bool isNull(std::vector<std::string> &vals);

  // memory
  std::vector<std::string> code;
  Memory memory;
};

#endif // MONET_INTERPRETER_H

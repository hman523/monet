/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Interpreter.h
 */

#ifndef MONET_INTERPRETER_H
#define MONET_INTERPRETER_H

#include "Exception.h"
#include "Memory.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <map>
#include <set>
#include <vector>

typedef boost::multiprecision::rational_adaptor<
    boost::multiprecision::cpp_int_backend<>>
    cpp_rational_backend;
typedef boost::multiprecision::number<cpp_rational_backend> num;

class Interpreter {
public:
  static Interpreter *Instance();
  static Interpreter *Instance(std::string filename);
  void interpret();
  void repl();

private:
  // Singleton functions
  static Interpreter *ptr;
  Interpreter();
  Interpreter(std::string filename);
  // helper functions

  std::vector<std::string> loadCodeFromFile(const std::string &filename);
  std::string eval(const std::string &value);
  std::string evalBuiltIns(const std::string &value,
                           const std::vector<std::string> &words);
  std::vector<std::string> split(const std::string &str,
                                 char delim = ' ') const;
  std::pair<std::string, std::string> listSplit(const std::string &list) const;
  bool isString(const std::string &val) const;
  bool isList(const std::string &val) const;

  int strToInt(const std::string &val) const;
  num strToNum(const std::string &val) const;
  bool strtobool(const std::string &val) const;
  std::string strtostr(const std::string &str) const;
  std::string strtolist(const std::string &val) const;
  std::string normalize(const std::string &val) const;

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
  bool isNull(const std::vector<std::string> &vals);

  // Library Functions
  bool isLibraryCall(const std::string &vals) const;
  std::string import(const std::vector<std::string> &vals);
  void includeLibrary(const std::string &libraryName);

  // Friend classes
  friend class File;
  friend class Time;

  // memory
  std::vector<std::string> code;
  Memory memory;

  // Parsing words
  const std::string FUNCTION_DECLARATION_NAME = "define";
  const std::string FUNCTION_END_NAME = "end";
  const std::string SUBROUTINE_DECLARATION_NAME = "subroutine";
  const std::string MEM_DECLARATION_NAME = "defmem";
};

#endif // MONET_INTERPRETER_H

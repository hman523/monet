/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Memory.h
 */

#ifndef MONET_MEMORY_H
#define MONET_MEMORY_H

#include "Exception.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <map>
#include <set>
#include <stack>
#include <vector>

#include "../lib/Library.h"

typedef boost::multiprecision::rational_adaptor<
    boost::multiprecision::cpp_int_backend<>>
    cpp_rational_backend;
typedef boost::multiprecision::number<cpp_rational_backend> num;

class Memory {
public:
  Memory();
  // getters
  std::string get(const std::string &var) const;
  std::string getstring(const std::string &var) const;
  bool getboolean(const std::string &var) const;
  num getnum(const std::string &var) const;
  std::string getlist(const std::string &var) const;

  std::vector<std::string> getfn(const std::string &var) const;

  // checks for functions
  bool functioninuse(const std::string &val) const;
  bool isBuiltInFn(const std::string &val) const;
  bool isFunction(const std::string &val) const;
  bool isSubroutine(const std::string &val) const;
  bool isMem(const std::string &val) const;
  bool isLibraryFn(const std::string &val) const;

  // creating functions
  void createfunction(const std::string &name,
                      const std::vector<std::string> &code);
  void createsub(const std::string &name, const std::vector<std::string> &code);
  void createmem(const std::string &name, const std::vector<std::string> &code);

  // creating primitives
  void createboolean(const std::string &name, bool value);
  void createnum(const std::string &name, num number);
  void createstring(const std::string &name, const std::string &str);
  void createlist(const std::string &name, const std::string &list);

  // checks for primitives
  bool boolexists(const std::string &var) const;
  bool strexists(const std::string &var) const;
  bool numexists(const std::string &var) const;
  bool listexists(const std::string &var) const;
  bool varexists(const std::string &var) const;

  // changing scope
  void enterfn();
  void enterfn(const std::vector<std::string> &parameters,
               const std::vector<std::string> &fndefinition);
  void leavefn();

  // memoize functions
  std::string *checkmem(const std::string &name,
                        const std::vector<std::string> &call);
  void insertmem(const std::string &name, const std::vector<std::string> &call,
                 const std::string &result);

  // for high order functions
  std::string getBinding(const std::string &var) const;
  std::string getType(const std::string &var) const;

  // For libraries
  bool libraryExists(const std::string &var) const;
  bool librayImported(const std::string &var) const;
  void importLibrary(const std::string &var);
  std::string libraryExec(const std::string &expression,
                          const std::string &lib);

private:
  num strtonum(const std::string &str) const;
  bool strtobool(const std::string &str) const;
  std::string strtostr(const std::string &str) const;
  std::string strtolist(const std::string &str) const;
  std::string to_string(const num &) const;
  void loadLibraries();

  std::stack<std::map<std::string, bool>> booleans;
  std::stack<std::map<std::string, num>> nums;
  std::stack<std::map<std::string, std::string>> strings;
  std::stack<std::map<std::string, std::string>> lists;
  std::map<std::string, std::vector<std::string>> functions;
  std::map<std::string, std::vector<std::string>> subroutines;
  std::map<std::string, std::vector<std::string>> mems;

  std::map<std::string, std::map<std::vector<std::string>, std::string>>
      memvalues;

  std::set<std::string> reservedwords;
  std::set<std::string> functionnamespace;
  std::set<std::string> subroutinenamespace;
  std::set<std::string> memnamespace;
  std::set<std::string> libraryfunctionnamespace;
  std::map<std::string, bool> libraries;
  std::map<std::string, Library *> libraryinstances;

  std::stack<std::map<std::string, std::string>> variabletypes;
  std::stack<std::map<std::string, std::string>> functionbindings;
};

#endif // MONET_MEMORY_H

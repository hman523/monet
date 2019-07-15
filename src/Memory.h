//
// Created by hunter on 6/29/19.
//

#ifndef MONET_MEMORY_H
#define MONET_MEMORY_H

#include <map>
#include <set>
#include <stack>
#include <vector>

typedef double num;

class Memory {
public:
  Memory();
  std::string get(std::string var);
  std::string getstring(std::string var);
  bool getboolean(std::string var);
  num getnum(std::string var);

  std::vector<std::string> getfn(std::string var);

  bool functioninuse(std::string val);
  bool isFunction(std::string val);
  bool isSubroutine(std::string val);
  bool isMem(std::string val);

  void createfunction(std::string name, std::vector<std::string> code);
  void createsub(std::string name, std::vector<std::string> code);
  void createmem(std::string name, std::vector<std::string> code);

  void createboolean(std::string name, bool value);
  void createnum(std::string name, num number);
  void createstring(std::string name, std::string str);

  bool boolexists(std::string var);
  bool strexists(std::string var);
  bool numexists(std::string var);
  bool varexists(std::string var);

  void enterfn();
  void enterfn(std::vector<std::string> parameters,
               std::vector<std::string> fndefinition);
  void leavefn();

  std::string *checkmem(std::string name, std::vector<std::string> call);
  void insertmem(std::string name, std::vector<std::string> call,
                 std::string result);

private:
  std::string getType(std::string var);

  num strtonum(std::string);
  bool strtobool(std::string);
  std::string strtostr(std::string);

  std::stack<std::map<std::string, bool>> booleans;
  std::stack<std::map<std::string, num>> nums;
  std::stack<std::map<std::string, std::string>> strings;
  std::map<std::string, std::vector<std::string>> functions;
  std::map<std::string, std::vector<std::string>> subroutines;
  std::map<std::string, std::vector<std::string>> mems;

  std::map<std::string, std::map<std::vector<std::string>, std::string>>
      memvalues;

  std::set<std::string> functionnamespace;
  std::set<std::string> subroutinenamespace;
  std::set<std::string> memnamespace;
  std::stack<std::map<std::string, std::string>> variabletypes;
  std::stack<std::map<std::string, std::string>> functionbindings;
};

#endif // MONET_MEMORY_H

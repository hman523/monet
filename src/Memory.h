//
// Created by hunter on 6/29/19.
//

#ifndef BIPL_MEMORY_H
#define BIPL_MEMORY_H

#include <map>
#include <set>

class Memory {
public:
  Memory();
  std::string get(std::string var);
  std::string getstring(std::string var);
  bool getboolean(std::string var);
  double getnum(std::string var);
  bool functioninuse(std::string val);
  void createboolean(std::string name, bool value);
  void createnum(std::string name, double num);
  void createstring(std::string name, std::string str);
  bool boolexists(std::string var);

private:
  std::map<std::string, bool> booleans;
  std::map<std::string, double> nums;
  std::map<std::string, std::string> strings;

  std::set<std::string> functionnamespace;
  std::map<std::string, std::string> variabletypes;
};

#endif // BIPL_MEMORY_H

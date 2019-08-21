/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: List.h
 */

#ifndef MONET_LIST_H
#define MONET_LIST_H

#include "../Library.h"
#include <vector>

class List : public Library {
public:
  List();
  virtual std::set<std::string> getFunctions();
  virtual std::string eval(const std::string &expression);

private:
  std::set<std::string> functions;
  std::vector<std::string> split(const std::string &text,
                                 const std::string &delims);
  std::string evalstring(const std::string &val);

  std::string nthTail(const std::vector<std::string> &vals);
};

#endif // MONET_LIST_H

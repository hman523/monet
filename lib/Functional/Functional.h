/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Functional.h
 */

#ifndef MONET_FUNCTIONAL_H
#define MONET_FUNCTIONAL_H

#include "../Library.h"
#include <vector>

class Functional : public Library {
public:
  Functional();
  std::set<std::string> getFunctions();
  std::string eval(const std::string &expression);

private:
  std::set<std::string> functions;
  bool isFunction(const std::string &fn);
  bool isList(const std::string &lst);
  std::string head(const std::string &lst);
  std::string tail(const std::string &lst);
  std::string strToList(const std::string &lst);

  std::string map(const std::vector<std::string> &expression);
  std::string apply(const std::vector<std::string> &expression);
  std::string reduce(const std::vector<std::string> &expression);
};

#endif // MONET_FUNCTIONAL_H

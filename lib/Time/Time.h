/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Time.h
 */

#ifndef MONET_TIME_H
#define MONET_TIME_H

#include "../Library.h"
#include <vector>

class Time : public Library {
public:
  Time();
  virtual std::set<std::string> getFunctions();
  virtual std::string eval(const std::string &expression);

private:
  std::set<std::string> functions;

  std::string time(const std::vector<std::string> &vals);
  std::string date(const std::vector<std::string> &vals);
  std::string local(const std::vector<std::string> &vals);
  std::string list(const std::vector<std::string> &vals);
  std::string stringtime(const std::vector<std::string> &vals);
};

#endif // MONET_TIME_H

//
// Created by hunter on 7/20/19.
//

#ifndef MONET_EXCEPTION_H
#define MONET_EXCEPTION_H

#include <string>

class Exception {
public:
  Exception() = delete;
  Exception(const std::string &ErrorMessage);
  std::string what();

private:
  const std::string errormessage;
};

#endif // MONET_EXCEPTION_H

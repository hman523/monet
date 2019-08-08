/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Math.h
 */

#ifndef MONET_MATH_H
#define MONET_MATH_H

#include "../Library.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <vector>
namespace bm = boost::multiprecision;
typedef boost::multiprecision::rational_adaptor<
    boost::multiprecision::cpp_int_backend<>>
    cpp_rational_backend;
typedef boost::multiprecision::number<cpp_rational_backend> num;

class Math : public Library {
public:
  Math();
  virtual std::set<std::string> getFunctions();
  virtual std::string eval(const std::string &expression);

private:
  std::set<std::string> functions;
  bool isNum(const std::string &val) const;
  num toNum(const std::string &val) const;
  std::string numToStr(const num &val) const;

  std::string exp(const std::vector<std::string> &vals);
};

#endif // MONET_MATH_H

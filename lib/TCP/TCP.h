/**
 * Stephen Hunter Barbella, Baibhav Vatsa
 * GitHub: hman523, baibhavatsa
 * Email: shbarbella@gmail.com, baibhavvatsa@gmail.com
 * Licence: MIT
 * File: TCP.h
 */

#ifndef MONET_TCP_H
#define MONET_TCP_H

#include "../Library.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <vector>
namespace bm = boost::multiprecision;
typedef boost::multiprecision::rational_adaptor<
    boost::multiprecision::cpp_int_backend<>>
    cpp_rational_backend;
typedef boost::multiprecision::number<cpp_rational_backend> num;

class TCP : public Library {
public:
  TCP();
  virtual std::set<std::string> getFunctions();
  virtual std::string eval(const std::string &expression);

private:
  std::set<std::string> functions;
  int evalint(const std::string &val);
  std::string evalstring(const std::string &val);
  std::string send(const std::vector<std::string> &expression);
  std::string rec(const std::vector<std::string> &expression);
};

#endif // MONET_TCP_H

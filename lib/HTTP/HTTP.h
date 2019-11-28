/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: HTTP.h
 */

#ifndef MONET_HTTP_H
#define MONET_HTTP_H

#include "../Library.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <vector>
namespace bm = boost::multiprecision;
typedef boost::multiprecision::rational_adaptor<
    boost::multiprecision::cpp_int_backend<>>
    cpp_rational_backend;
typedef boost::multiprecision::number<cpp_rational_backend> num;

class HTTP : public Library {
public:
  HTTP();
  virtual std::set<std::string> getFunctions();
  virtual std::string eval(const std::string &expression);

private:
  std::set<std::string> functions;

  std::string evalstring(const std::string &val);

  std::string rest(const std::vector<std::string> &vals,
                   const std::string &method,
                   const std::basic_string<char> methodc);
};

#endif // MONET_HTTP_H

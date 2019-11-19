//
// Created by hunter on 11/18/19.
//

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
  std::string get(const std::vector<std::string> &vals);
  std::string head(const std::vector<std::string> &vals);
  std::string put(const std::vector<std::string> &vals);
  std::string post(const std::vector<std::string> &vals);
  std::string del(const std::vector<std::string> &vals);
};

#endif // MONET_HTTP_H

/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: HTTP.cpp
 */

#include "HTTP.h"
#include "../../include/Interpreter.h"
#include <cpprest/http_client.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

HTTP::HTTP()
    : functions(
          {"http.get", "http.post", "http.put", "http.head", "http.delete"}) {}

std::set<std::string> HTTP::getFunctions() { return functions; }

std::string HTTP::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "http.get") {
    return rest(words, "http.get", methods::GET);
  } else if (words[0] == "http.post") {
    return rest(words, "http.post", methods::POST);
  } else if (words[0] == "http.put") {
    return rest(words, "http.put", methods::PUT);
  } else if (words[0] == "http.head") {
    return head(words);
  } else if (words[0] == "http.delete") {
    return rest(words, "http.delete", methods::DEL);
  }
  throw Exception("Fatal implementation error in HTTP library in HTTP.cpp");
}

std::string HTTP::evalstring(const std::string &val) {
  if (Interpreter::Instance()->isParens(val)) {
    return Interpreter::Instance()->eval(val);
  } else if (Interpreter::Instance()->isString(val)) {
    return Interpreter::Instance()->removequotes(val);
  } else {
    return Interpreter::Instance()->strtostr(val);
  }
}

std::string HTTP::rest(const std::vector<std::string> &vals,
                       const std::string &method,
                       const std::basic_string<char> methodc) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters in " + method);
  }
  std::string url = evalstring(vals[1]);
  http_client client(U(url));
  http_response response = client.request(methodc).get();
  if (response.status_code() == status_codes::OK) {
    std::string reply = response.extract_string().get();
    return reply;
  } else {
    throw Exception("OK not returned from " + method + " request");
  }
}

std::string HTTP::head(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters in http.head");
  }
  std::string url = evalstring(vals[1]);
  http_client client(U(url));
  http_response response = client.request(methods::HEAD).get();
  if (response.status_code() == status_codes::OK) {
    std::string lst = "[\"";
    for (auto j = response.headers().begin(); j != response.headers().end();
         ++j) {
      auto f = j.operator*().first;
      auto s = j.operator*().second;
      lst += f + "\", \"" + s + "\", \"";
    }
    lst = lst.substr(0, lst.length() - 3);
    lst += "]";
    return lst;
  } else {
    throw Exception("OK not returned from http.head request");
  }
}

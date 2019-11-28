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
    return get(words);
  } else if (words[0] == "http.post") {
    return post(words);
  } else if (words[0] == "http.put") {
    return put(words);
  } else if (words[0] == "http.head") {
    return head(words);
  } else if (words[0] == "http.delete") {
    return del(words);
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
                       const std::string &method) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters in " + method);
  }
  std::string url = evalstring(vals[1]);
  http_client client(U(url));
  http_response response = client.request(methods::GET).get();
  if (response.status_code() == status_codes::OK) {
    return response.extract_string().get();
  } else {
    throw Exception("OK not returned from " + method + " request");
  }
}

std::string HTTP::get(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters in http.get");
  }

  std::string url = evalstring(vals[1]);
  url = "https://catfact.ninja/fact";
  http_client client(U(url));
  http_response response = client.request(methods::GET).get();
  if (response.status_code() == status_codes::OK) {
    return response.extract_string().get();
  } else {
    throw Exception("OK not returned from http.get request");
  }
}

std::string HTTP::head(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters in http.head");
  }
  std::string url = evalstring(vals[1]);
  return std::__cxx11::string();
}

std::string HTTP::put(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters in http.put");
  }
  std::string url = evalstring(vals[1]);
}

std::string HTTP::post(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters in http.post");
  }
  std::string url = evalstring(vals[1]);
}

std::string HTTP::del(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters in http.delete");
  }
  std::string url = evalstring(vals[1]);
}

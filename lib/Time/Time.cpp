/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Time.cpp
 */

#include "Time.h"
#include "../../include/Exception.h"
#include "../../include/Interpreter.h"
#include <ctime>

Time::Time()
    : functions({"time.time", "time.date", "time.local", "time.localdate",
                 "time.list", "time.string", "time.localstring",
                 "time.locallist"}) {}

std::set<std::string> Time::getFunctions() { return functions; }

std::string Time::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "time.time") {
    return time(words);
  } else if (words[0] == "time.date") {
    return date(words);
  } else if (words[0] == "time.local") {
    return local(words);
  } else if (words[0] == "time.localdate") {
    return localdate(words);
  } else if (words[0] == "time.list") {
    return list(words);
  } else if (words[0] == "time.locallist") {
    return locallist(words);
  } else if (words[0] == "time.string") {
    return stringtime(words);
  } else if (words[0] == "time.localstring") {
    return localstringtime(words);
  }
  throw Exception("Fatal implementation error in Time library in Time.cpp");
}

std::string Time::time(const std::vector<std::string> &vals) {
  if (vals.size() != 1) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
  time_t t;
  std::time(&t);
  return std::to_string(t);
}

std::string Time::date(const std::vector<std::string> &vals) {
  if (vals.size() != 1) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
  time_t t = std::time(0);
  tm *gmt = gmtime(&t);
  return std::string(std::to_string(1900 + gmt->tm_year) + "/" +
                     std::to_string(1 + gmt->tm_mon) + "/" +
                     std::to_string(gmt->tm_mday));
}

std::string Time::localdate(const std::vector<std::string> &vals) {
  if (vals.size() != 1) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
  time_t t = std::time(0);
  tm *m = std::localtime(&t);
  return std::string(std::to_string(1900 + m->tm_year) + "/" +
                     std::to_string(1 + m->tm_mon) + "/" +
                     std::to_string(m->tm_mday));
}

std::string Time::local(const std::vector<std::string> &vals) {
  if (vals.size() != 1) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
  time_t t = std::time(0);
  tm *m = std::localtime(&t);
  return std::string(std::to_string(m->tm_hour) + ":" +
                     std::to_string(m->tm_min) + ":" +
                     std::to_string(m->tm_sec));
}

std::string Time::locallist(const std::vector<std::string> &vals) {
  if (vals.size() != 1) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
  time_t t = std::time(0);
  tm *m = gmtime(&t);
  return std::string(
      "[" + std::to_string(1900 + m->tm_year) + " " +
      std::to_string(1 + m->tm_mon) + " " + std::to_string(m->tm_mday) + " " +
      std::to_string(m->tm_hour) + " " + std::to_string(m->tm_min) + " " +
      std::to_string(m->tm_sec) + "]");
}

std::string Time::list(const std::vector<std::string> &vals) {
  if (vals.size() != 1) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
  time_t t = std::time(0);
  tm *m = std::localtime(&t);
  return std::string(
      "[" + std::to_string(1900 + m->tm_year) + " " +
      std::to_string(1 + m->tm_mon) + " " + std::to_string(m->tm_mday) + " " +
      std::to_string(m->tm_hour) + " " + std::to_string(m->tm_min) + " " +
      std::to_string(m->tm_sec) + "]");
}

std::string Time::stringtime(const std::vector<std::string> &vals) {
  if (vals.size() != 1) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
  if (vals.size() != 1) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
  time_t t = std::time(0);
  tm *gmt = gmtime(&t);
  std::string str = std::__cxx11::string(asctime(gmt));
  return str.substr(0, str.length() - 1);
}

std::string Time::localstringtime(const std::vector<std::string> &vals) {
  if (vals.size() != 1) {
    throw Exception("Wrong number of parameters for " + vals[0]);
  }
  time_t t = std::time(0);
  std::string str = std::__cxx11::string(std::ctime(&t));
  return str.substr(0, str.length() - 1);
}

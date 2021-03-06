/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: File.h
 */

#ifndef MONET_FILE_H
#define MONET_FILE_H

#include "../Library.h"
#include <vector>

class File : public Library {
public:
  File();
  virtual std::set<std::string> getFunctions();
  virtual std::string eval(const std::string &expression);

private:
  std::set<std::string> functions;

  std::vector<std::string> split(const std::string &text,
                                 const std::string &delims);
  std::string evalstring(const std::string &val);

  std::string readfile(const std::vector<std::string> &vals);
  std::string writefile(const std::vector<std::string> &vals);
  std::string getline(const std::vector<std::string> &vals);
  std::string wordsfile(const std::vector<std::string> &vals);
  std::string splitfile(const std::vector<std::string> &vals);
  std::string getlastline(const std::vector<std::string> &vals);
  std::string appendtofile(const std::vector<std::string> &vals);
  std::string fileexists(const std::vector<std::string> &vals);
  std::string linecount(const std::vector<std::string> &vals);
};

#endif // MONET_FILE_H

//
// Created by hunter on 7/31/19.
//

#ifndef MONET_FILE_H
#define MONET_FILE_H

#include "Library.h"

class File : public Library {
public:
  File();
  virtual std::set<std::string> getFunctions();
  virtual std::string eval(const std::string &expression);

private:
  std::set<std::string> functions;

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

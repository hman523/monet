//
// Created by hunter on 7/31/19.
//

#ifndef MONET_FILE_H
#define MONET_FILE_H

#include "Library.h"

class File : public Library{
public:
    File();
    virtual std::set<std::string> getFunctions();
    virtual std::string eval(const std::string &expression);

private:
    std::set<std::string> functions;
    friend std::string eval(const std::string &value);
    friend std::vector<std::string> split(const std::string &val);
    friend std::string strtostr(const std::string &str);
    friend std::string strtolist(const std::string &val);
    friend std::string normalize(const std::string &val);

    std::string readfile(const std::vector<std::string> &vals);

};


#endif //MONET_FILE_H

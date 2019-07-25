//
// Created by hunter on 7/24/19.
//

#ifndef BIPL_FILE_H
#define BIPL_FILE_H


#include <set>
#include <string>
#include <vector>
#include "../src/Interpreter.h"

class File : public Library{
public:
    File();
    std::set<std::string> getFunctions();
    std::string eval(const std::string &expression);
private:
    friend std::string Interpreter::eval(const std::string &value);
    friend std::vector<std::string> Interpreter::split(const std::string &str, char delim) const;
    std::set<std::string> functions;
    Interpreter * interpreter;

    std::string cat(std::vector<std::string> words);
};




#endif //BIPL_FILE_H

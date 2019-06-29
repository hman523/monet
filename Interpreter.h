//
// Created by hunter on 6/29/19.
//

#ifndef BIPL_INTERPRETER_H
#define BIPL_INTERPRETER_H

#include <iostream>
#include <vector>
#include <map>
#include <set>

class Interpreter {
public:
    Interpreter() = delete;
    Interpreter(std::string filename);

private:

    //functions
    void printcode();
    void interpret();
    void eval(std::string value);
    std::vector<std::string> split(std::string str, char delim = ' ');

    //memory
    std::vector<std::string> code;
    std::map<std::string, bool> booleans;
    std::map<std::string, double> nums;
    std::map<std::string, std::string> strings;
    std::set<std::string> reservedwords;
};


#endif //BIPL_INTERPRETER_H

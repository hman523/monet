//
// Created by hunter on 6/29/19.
//

#ifndef BIPL_INTERPRETER_H
#define BIPL_INTERPRETER_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "Memory.h"

class Interpreter {
public:
    Interpreter() = delete;
    Interpreter(std::string filename);

private:

    //helper functions
    void interpret();
    void eval(std::string value);
    std::vector<std::string> split(std::string str, char delim = ' ');
    bool isString(std::string val);
    int strtoint(std::string num);
    double strtonum(std::string num);
    std::string removequotes(std::string original);

    //command functions for the interpretting
    void printcode();
    void quit(std::vector<std::string> vals);
    void print(std::vector<std::string> vals);



    //memory
    std::vector<std::string> code;
    Memory memory;
};


#endif //BIPL_INTERPRETER_H

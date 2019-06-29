//
// Created by hunter on 6/29/19.
//

#include "Interpreter.h"
#include <fstream>
#include <algorithm>
#include <iterator>

Interpreter::Interpreter(std::string filename) {
    std::ifstream infile;
    infile.open(filename);
    std::string line;
    while(std::getline(infile, line)){
        code.push_back(line);
    }
    interpret();
}



void Interpreter::interpret() {
    std::for_each(code.begin(), code.end(), [&](std::string line)->void { eval(line);});
}

void Interpreter::eval(std::string value) {
    std::vector<std::string> words = split(value);
    if (words.size() == 0){
        return;
    }
    if (words[0] == "quit"){

    }
}

void Interpreter::printcode() {
    std::copy(code.begin(), code.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
}

std::vector<std::string> Interpreter::split(std::string str, char delim) {
    std::vector<std::string> returnval;
    std::string temp = "";
    bool instr = false;
    for (uint32_t i = 0; i < str.length(); ++i) {
        if(str[i] == '"'){
            instr = !instr;
        }
        if(str[i] == delim && temp != "" && !instr){
            returnval.push_back(temp);
            temp = "";
        }
        else if (instr){
            temp+=str[i];
        }
        else{
            temp+=tolower(str[i]);
        }
    }
    if (temp!= ""){
        returnval.push_back(temp);
    }
    return returnval;
}
//
// Created by hunter on 7/31/19.
//

#include <vector>
#include <fstream>
#include "File.h"
#include "../src/Exception.h"
#include "../src/Interpreter.h"

File::File() {
    functions.insert({"file.read", "file.write"});
}

std::set<std::string> File::getFunctions() {
    return functions;
}

std::string File::eval(const std::string &expression) {
    std::vector<std::string> words = Interpreter::Instance()->split(expression);
    if(words[0] == "file.read"){
        return readfile(words);
    }else if(words[0] == "file.write"){
        return writefile(words);
    }else if(words[0] == "file.exists"){
        return fileexists(words);
    }else if(words[0] == "file.getline"){
        return getline(words);
    }else if(words[0] == "file.words"){
        return wordsfile(words);
    }else if(words[0] == "file.split"){
        return splitfile(words);
    }else if(words[0] == "file.getlast"){
        return getlastline(words);
    }else if(words[0] == "file.append"){
        return appendtofile(words);
    }
    return "";
}

std::string File::readfile(const std::vector<std::string> &vals){
    if (vals.size() != 2){
        throw Exception("Wrong number of parameters for readfile");
    }
    std::string filename;
    if (Interpreter::Instance()->isParens(vals[1])){
        filename = Interpreter::Instance()->eval(vals[1]);
    }
    else if(Interpreter::Instance()->isString(vals[1])){
        filename = Interpreter::Instance()->removequotes(vals[1]);
    }
    else{
        filename = Interpreter::Instance()->strtostr(vals[1]);
    }
    std::ifstream file(filename);
    std::string text((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    return text;

}

std::string File::writefile(const std::vector<std::string> &vals) {
    if (vals.size() != 3){
        throw Exception("Wrong number of parameters for readfile");
    }
    std::string filename;
    if (Interpreter::Instance()->isParens(vals[1])){
        filename = Interpreter::Instance()->eval(vals[1]);
    }
    else if(Interpreter::Instance()->isString(vals[1])){
        filename = Interpreter::Instance()->removequotes(vals[1]);
    }
    else{
        filename = Interpreter::Instance()->strtostr(vals[1]);
    }
    std::string filecontents;
    if (Interpreter::Instance()->isParens(vals[2])){
        filecontents = Interpreter::Instance()->eval(vals[2]);
    }
    else if(Interpreter::Instance()->isString(vals[2])){
        filecontents = Interpreter::Instance()->removequotes(vals[2]);
    }
    else{
        filecontents = Interpreter::Instance()->strtostr(vals[2]);
    }
    std::ofstream out(filename);
    out << filecontents;
    out.close();
    return "";
}

std::string File::getline(const std::vector<std::string> &vals) {
    return std::__cxx11::string();
}

std::string File::wordsfile(const std::vector<std::string> &vals) {
    return std::__cxx11::string();
}

std::string File::splitfile(const std::vector<std::string> &vals) {
    return std::__cxx11::string();
}

std::string File::getlastline(const std::vector<std::string> &vals) {
    return std::__cxx11::string();
}

std::string File::appendtofile(const std::vector<std::string> &vals) {
    return std::__cxx11::string();
}

std::string File::fileexists(const std::vector<std::string> &vals) {
    return std::__cxx11::string();
}

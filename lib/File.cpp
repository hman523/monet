//
// Created by hunter on 7/24/19.
//

#include "File.h"

File::File(Interpreter *i) : interpreter(i){
    functions.insert({"cat"});
}

std::set<std::string> File::getFunctions() {
    return functions;
}

std::string File::eval(const std::string& expression) {
    std::vector<std::string> words = interpreter->split(expression);
}

std::string File::cat(std::vector<std::string> words) {
    if(words.size() != 2){
        throw Exception("cat requires 2 parameters");
    }
}

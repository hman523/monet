//
// Created by hunter on 6/29/19.
//

#include <sstream>
#include "Memory.h"

std::string Memory::get(std::string var) {
    if(variabletypes[var] == "boolean"){
        if(booleans[var]){
            return "true";
        } else{
            return "false";
        }
    }
    if (variabletypes[var] == "num"){
        return std::to_string(nums[var]);
    }
    if (variabletypes[var] == "string"){
        return strings[var];
    }
    else{
        throw std::logic_error("Variable " + var + " not found");
    }
}

double Memory::strtonum(std::string num) {
    std::stringstream ss(num);
    double val;
    ss >> val;
    return val;
}

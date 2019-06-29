//
// Created by hunter on 6/29/19.
//

#ifndef BIPL_MEMORY_H
#define BIPL_MEMORY_H

#include <map>
#include <set>

class Memory {
public:
    Memory() = default;
    std::string get(std::string var);


private:
    double strtonum(std::string);

    std::map<std::string, bool> booleans;
    std::map<std::string, double> nums;
    std::map<std::string, std::string> strings;

    std::set<std::string> functionnamespace;
    std::map<std::string, std::string> variabletypes;
};


#endif //BIPL_MEMORY_H

//
// Created by hunter on 7/24/19.
//

#ifndef MONET_LIBRARY_H
#define MONET_LIBRARY_H

#include <set>
#include <string>

class Library{
    virtual std::set<std::string> getFunctions() = 0;
    virtual std::string eval(const std::string &expression) = 0;
};

#endif //MONET_LIBRARY_H

//
// Created by hunter on 7/20/19.
//

#include "Exception.h"

Exception::Exception(const std::string &ErrorMessage)
    : errormessage(ErrorMessage) {}

std::string Exception::what() { return errormessage; }

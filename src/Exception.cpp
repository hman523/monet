/**
 * Stephen Hunter Barbella
 * GitHub: hman523
 * Email: shbarbella@gmail.com
 * Licence: MIT
 * File: Exception.cpp
 */

#include "Exception.h"

Exception::Exception(const std::string &ErrorMessage)
    : errormessage(ErrorMessage) {}

std::string Exception::what() { return errormessage; }

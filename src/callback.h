#pragma once

#include <string>
#include <deque>
#include <variant>

#include "common.h"


class Callback 
{
public:
    Callback(AMX* amx, const std::string& function, const char* format, cell* params, unsigned int param_offset);
    void setResult(int value);
    AMX* getAMX() const;
    bool call();

private:
    AMX* amx;
    std::string function;
    std::deque<std::variant<int, std::string>> arguments;
    int result;
};
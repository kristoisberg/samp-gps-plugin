#pragma once

#include <string>
#include <deque>
#include <variant>

#include "../lib/sdk/amx/amx2.h"


class Callback {
public:
    Callback(AMX* amx, std::string function, const char* format, cell* params, unsigned int param_offset);
    void setResult(int result);
    AMX* getAMX();
    bool call();

private:
    AMX* amx;
    std::string function;
    std::deque<std::variant<int, std::string>> arguments;
    int result;
};
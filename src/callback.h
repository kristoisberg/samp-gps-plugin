#pragma once

#include <string>
#include <deque>
#include <variant>

#include "common.h"


class Callback
{
public:
	Callback(Amx* amx, const std::string& function, const char* format, cell* params, unsigned int param_offset);
	void setResult(int value);
	Amx* getAmx() const;
	bool call();

private:
	Amx* amx_;
	std::string function_;
	std::deque<std::variant<int, std::string>> arguments_;
	int result_;
};

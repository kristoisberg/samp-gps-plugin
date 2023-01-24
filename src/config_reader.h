#pragma once

#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>


class ConfigReader
{
public:
	ConfigReader(const std::string& filename);

	template <typename T>
	T getValue(const std::string& key, T defaultValue)
	{
		if (!hasServerCfg_)
		{
			return defaultValue;
		}

		file_.clear();
		file_.seekg(0, std::ios::beg);

		std::string buffer, line_key;
		T value;

		while (std::getline(file_, buffer))
		{
			std::istringstream input(buffer);

			input >> line_key;

			if (key == line_key)
			{
				input >> value;
				return value;
			}
		}

		return defaultValue;
	}

private:
	std::ifstream file_;
	bool hasServerCfg_ = false;
};
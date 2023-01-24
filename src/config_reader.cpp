#include <fstream>
#include <sstream>

#include "config_reader.h"


ConfigReader::ConfigReader(const std::string& filename)
{
	file_ = std::ifstream(filename);

	if (!file_.good())
	{
		hasServerCfg_ = false;
	}
	else
	{
		hasServerCfg_ = true;
	}
}

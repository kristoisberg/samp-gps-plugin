#include <fstream>
#include <sstream>

#include "config_reader.h"


ConfigReader::ConfigReader(const std::string& filename)
{
	file_ = std::ifstream(filename);

	if (!file_.good())
	{
		throw std::runtime_error("Could not load configuration file " + filename + "!");
	}
}

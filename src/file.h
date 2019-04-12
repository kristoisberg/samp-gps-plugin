#pragma once

#include <string>


namespace File 
{
    bool LoadNodes(const std::string& filename, int& line_count, int& node_count, int& connection_count);
};
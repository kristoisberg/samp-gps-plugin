#include <fstream>
#include <sstream>

#include "file.h"
#include "container.h"
#include "connection.h"
#include <iostream>


namespace File
{
	bool LoadNodes(const std::string& filename, int& line_count, int& node_count, int& connection_count)
	{
		std::ifstream file("scriptfiles/" + filename);

		if (!file.good())
		{
			return false;
		}

		Container::LockExclusive();

		std::string buffer;
		int type = 0, id = 0, id2 = 0, direction = 0, ignore = 0;
		float x = 0.0f, y = 0.0f, z = 0.0f;

		while (std::getline(file, buffer))
		{
			std::istringstream input(buffer);

			input >> type;

			switch (type)
			{
			case 0:
			{
				input >> x >> y >> z >> ignore >> id;

				if (Container::Nodes::Add(id, x, y, z))
				{
					node_count++;
				}

				break;
			}

			case 1:
			{
				input >> id >> id2 >> direction;

				if (direction != 2 && Container::Connections::Add(id, id2))
				{
					connection_count++;
				}

				break;
			}
			}

			line_count++;
		}

		Container::UnlockExclusive();

		file.close();
		return true;
	}


	bool SaveNodes(const std::string& filename)
	{
		std::ofstream file("scriptfiles/" + filename);

		if (!file.good())
		{
			return false;
		}

		for (const auto node : Container::Nodes::GetAll())
		{
			if (!node.second->isSetForDeletion())
			{
				file << 0 << " " << node.second->getX() << " " << node.second->getY() << " " << node.second->getZ() << " " << -1 << " " << node.second->getID() << '\n';
			}
		}

		for (const auto connection : Container::Connections::GetAll())
		{
			file << 1 << " " << connection.second->getSource()->getID() << " " << connection.second->getTarget()->getID() << '\n';
		}

		file.close();
		return true;
	}
};
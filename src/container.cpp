#include "container.h"
#include "connection.h"


namespace Container
{
	namespace
	{
		std::unordered_map<int, Node*> nodes;
		int highest_node_id;

		std::unordered_map<int, Path*> paths;
		int highest_path_id;
	}


	bool AddNode(const int id, const float x, const float y, const float z)
	{
		if (GetNodeByID(id) != nullptr)
		{
			return false;
		}

		nodes[id] = new Node(id, x, y, z);

		if (id > highest_node_id)
		{
			highest_node_id = id;
		}

		return true;
	}


	bool AddConnection(const int startID, const int targetID)
	{
		auto start = GetNodeByID(startID);

		if (start == nullptr)
		{
			return false;
		}

		const auto target = GetNodeByID(targetID);

		if (target == nullptr)
		{
			return false;
		}

		start->addConnection(new Connection(target, start->getDistanceFromNode(target)));
		return true;
	}


	Node* GetNodeByID(const int id)
	{
		const auto node = nodes.find(id);

		if (node == nodes.end())
		{
			return nullptr;
		}

		return node->second;
	}


	std::unordered_map<int, Node*> GetNodes()
	{
		return nodes;
	}


	int GetHighestNodeID()
	{
		return highest_node_id;
	}


	int AddPath(Path* path)
	{
		const auto id = highest_path_id++;

		paths[id] = path;
		path->setID(id);

		return id;
	}

	void RemovePath(Path* path)
	{
		paths.erase(path->getID());

		delete path;
	}


	Path* GetPathByID(const int id)
	{
		const auto path = paths.find(id);

		if (path == paths.end())
		{
			return nullptr;
		}

		return path->second;
	}


	std::unordered_map<int, Path*> GetPaths()
	{
		return paths;
	}


	int GetHighestPathID()
	{
		return highest_path_id;
	}
}
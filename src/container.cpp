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


	void SetNodeForDeletion(Node* node)
	{
		if (!TryDeleteNode(node))
		{
			node->setForDeletion();
		}
	}


	bool TryDeleteNode(Node* node)
	{
		if (node->getPathCount() > 0)
		{
			return false;
		}

		for (auto connection : node->getOutgoingConnections())
		{
			DeleteConnection(connection);
		}

		for (auto connection : node->getIncomingConnections())
		{
			DeleteConnection(connection);
		}

		nodes.erase(node->getID());

		delete node;
		return true;
	}


	bool AddConnection(const int sourceID, const int targetID)
	{
		auto source = GetNodeByID(sourceID);

		if (source == nullptr)
		{
			return false;
		}

		const auto target = GetNodeByID(targetID);

		if (target == nullptr)
		{
			return false;
		}

		const auto connection = new Connection(source, target, source->getDistanceFromNode(target));

		source->addOutgoingConnection(connection);
		target->addIncomingConnection(connection);
		
		return true;
	}


	void DeleteConnection(Connection* connection)
	{
		connection->getSource()->removeOutgoingConnection(connection);
		connection->getTarget()->removeIncomingConnection(connection);

		delete connection;
	}


	int AddPath(Path* path)
	{
		const auto id = highest_path_id++;

		paths[id] = path;
		path->setID(id);

		return id;
	}

	void DeletePath(Path* path)
	{
		for (auto node : path->getNodes())
		{
			node->removeFromPath(path);

			if (node->isSetForDeletion())
			{
				TryDeleteNode(node);
			}
		}

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
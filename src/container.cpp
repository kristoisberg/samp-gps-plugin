#include <shared_mutex>

#include "container.h"
#include "connection.h"


namespace Container
{
	namespace
	{
		std::unordered_map<int, Connection*> connections;
		int highest_connection_id;

		std::unordered_map<int, Node*> nodes;
		int highest_node_id;

		std::unordered_map<int, Path*> paths;
		int highest_path_id;

		std::shared_mutex lock;
	}


	void LockExclusive()
	{
		lock.lock();
	}


	void UnlockExclusive()
	{
		lock.unlock();
	}


	void LockShared()
	{
		lock.lock_shared();
	}


	void UnlockShared()
	{
		lock.unlock_shared();
	}


	namespace Nodes
	{
		int AllocateID()
		{
			return ++highest_node_id;
		}

		bool Add(const int id, const float x, const float y, const float z)
		{
			if (Find(id) != nullptr)
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

		Node* Find(const int id)
		{
			const auto node = nodes.find(id);

			if (node == nodes.end())
			{
				return nullptr;
			}

			return node->second;
		}

		std::unordered_map<int, Node*> GetAll()
		{
			return nodes;
		}

		int GetHighestID()
		{
			return highest_node_id;
		}

		void SetForDeletion(Node* node)
		{
			if (!TryDelete(node))
			{
				node->setForDeletion();
			}
		}

		bool TryDelete(Node* node)
		{
			if (node->getPathCount() > 0)
			{
				return false;
			}

			for (auto connection : node->getConnections())
			{
				Connections::Delete(connection);
			}

			nodes.erase(node->getID());

			delete node;
			return true;
		}
	}


	namespace Connections
	{
		int Add(const int sourceID, const int targetID)
		{
			if (sourceID == targetID)
			{
				return -1;
			}

			auto source = Nodes::Find(sourceID);

			if (source == nullptr)
			{
				return -1;
			}

			const auto target = Nodes::Find(targetID);

			if (target == nullptr)
			{
				return -1;
			}

			const auto id = highest_connection_id++;
			const auto connection = new Connection(id, source, target, source->getDistanceFromNode(target));

			connections[id] = connection;

			source->addConnection(connection);
			target->addConnection(connection);

			return id;
		}

		void Delete(Connection* connection)
		{
			connection->getSource()->removeConnection(connection);
			connection->getTarget()->removeConnection(connection);

			delete connection;
		}

		Connection* Find(int id)
		{
			const auto connection = connections.find(id);

			if (connection == connections.end())
			{
				return nullptr;
			}

			return connection->second;
		}

		std::unordered_map<int, Connection*> GetAll()
		{
			return connections;
		}

		int GetHighestID()
		{
			return highest_connection_id;
		}
	}


	namespace Paths
	{
		int Add(Path* path)
		{
			const auto id = highest_path_id++;

			paths[id] = path;
			path->setID(id);

			return id;
		}

		void Delete(Path* path)
		{
			for (auto node : path->getNodes())
			{
				node->removeFromPath(path);

				if (node->isSetForDeletion())
				{
					Nodes::TryDelete(node);
				}
			}

			paths.erase(path->getID());

			delete path;
		}

		Path* Find(const int id)
		{
			const auto path = paths.find(id);

			if (path == paths.end())
			{
				return nullptr;
			}

			return path->second;
		}

		std::unordered_map<int, Path*> GetAll()
		{
			return paths;
		}

		int GetHighestID()
		{
			return highest_path_id;
		}
	}
}
#pragma once

#include <unordered_map>

#include "path.h"


namespace Container
{
	void LockExclusive();
	void UnlockExclusive();

	void LockShared();
	void UnlockShared();

	namespace Nodes
	{
		int AllocateID();
		bool Add(int id, float x, float y, float z);
		Node* Find(int id);
		std::unordered_map<int, Node*> GetAll();
		int GetHighestID();
		void SetForDeletion(Node* node);
		bool TryDelete(Node* node);
	}

	namespace Connections
	{
		int Add(int sourceID, int targetID);
		void Delete(Connection* connection);
		Connection* Find(int id);
		std::unordered_map<int, Connection*> GetAll();
		int GetHighestID();
	}
	
	namespace Paths
	{
		int Add(Path* path);
		void Delete(Path* path);
		Path* Find(int id);
		std::unordered_map<int, Path*> GetAll();
		int GetHighestID();
	}
};

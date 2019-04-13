#pragma once

#include <unordered_map>

#include "path.h"


namespace Container
{
	bool AddNode(int id, float x, float y, float z);
	
	Node* GetNodeByID(int id);
	std::unordered_map<int, Node*> GetNodes();
	int GetHighestNodeID();
	void SetNodeForDeletion(Node* node);
	bool TryDeleteNode(Node* node);

	bool AddConnection(int sourceID, int targetID);
	void DeleteConnection(Connection* connection);

	int AddPath(Path* path);
	void DeletePath(Path* path);
	Path* GetPathByID(int id);
	std::unordered_map<int, Path*> GetPaths();
	int GetHighestPathID();
};

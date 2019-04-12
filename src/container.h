#pragma once

#include <unordered_map>

#include "path.h"


namespace Container
{
	bool AddNode(int id, float x, float y, float z);
	bool AddConnection(int startID, int targetID);
	Node* GetNodeByID(int id);
	std::unordered_map<int, Node*> GetNodes();
	int GetHighestNodeID();

	int AddPath(Path* path);
	void RemovePath(Path* path);
	Path* GetPathByID(int id);
	std::unordered_map<int, Path*> GetPaths();
	int GetHighestPathID();
};

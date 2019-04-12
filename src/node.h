#pragma once

#include <vector>
#include "common.h"


class Node
{
public:
	Node(int id, float x, float y, float z);

	void setForDeletion();
	bool isSetForDeletion() const;

	void addConnection(Connection* connection);
	std::vector<Connection*> getConnections() const;

	void addToPath(Path* path, int index);
	void removeFromPath(Path* path);
	int getPositionInPath(Path* path);
	int getPathCount() const;

	int getID() const;

	float getX() const;
	float getY() const;
	float getZ() const;

	float getDistanceFromPoint(float pointX, float pointY, float pointZ) const;
	float getDistanceFromNode(Node* other) const;

	float getAngleFromPoint(float pointX, float pointY) const;
	float getAngleFromNode(Node* other) const;

private:
	int id;
	bool willBeDeleted;

	float x;
	float y;
	float z;

	std::vector<Connection*> connections;
	std::vector<std::pair<Path*, int>> paths;
};
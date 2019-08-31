#pragma once

#include <vector>
#include "common.h"
#include <mutex>


class Node
{
public:
	Node(int id, float x, float y, float z);

	void setForDeletion();
	bool isSetForDeletion() const;

	void addConnection(Connection* connection);
	void removeConnection(Connection* connection);
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
	int id_;
	bool is_set_for_deletion_;

	float x_;
	float y_;
	float z_;

	std::vector<Connection*> connections_;

	std::vector<std::pair<Path*, int>> paths_;
	std::mutex paths_lock_;
};

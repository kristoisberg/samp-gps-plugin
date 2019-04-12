#include <cmath>

#include "node.h"
#include <algorithm>


Node::Node(const int id, const float x, const float y, const float z)
{
	this->id = id;
	this->willBeDeleted = false;

	this->x = x;
	this->y = y;
	this->z = z;
}


void Node::setForDeletion()
{
	this->willBeDeleted = true;
}


bool Node::isSetForDeletion() const
{
	return this->willBeDeleted;
}


void Node::addConnection(Connection* connection)
{
	this->connections.push_back(connection);
}


std::vector<Connection*> Node::getConnections() const
{
	return this->connections;
}


void Node::addToPath(Path* path, int index)
{
	this->paths.emplace_back(path, index);
}


int Node::getPositionInPath(Path* path)
{
	const auto it = std::find_if(this->paths.begin(), this->paths.end(), [&path](const std::pair<Path*, int>& pair) { return pair.first == path; });

	if (it != this->paths.end())
	{
		return it->second;
	}

	return -1;
}


void Node::removeFromPath(Path* path)
{
	const auto it = std::find_if(this->paths.begin(), this->paths.end(), [&path](const std::pair<Path*, int>& pair) { return pair.first == path; });

	if (it != this->paths.end())
	{
		this->paths.erase(it);
	}
}


int Node::getPathCount() const
{
	return this->paths.size();
}


int Node::getID() const
{
	return this->id;
}


float Node::getX() const
{
	return this->x;
}


float Node::getY() const
{
	return this->y;
}


float Node::getZ() const
{
	return this->z;
}


float Node::getDistanceFromPoint(const float pointX, const float pointY, const float pointZ) const
{
	return sqrt(pow(pointX - x, 2.0f) + pow(pointY - y, 2.0f) + pow(pointZ - z, 2.0f));
}


float Node::getDistanceFromNode(Node* other) const
{
	return getDistanceFromPoint(other->getX(), other->getY(), other->getZ());
}


float Node::getAngleFromPoint(const float pointX, const float pointY) const
{
	return atan2(pointY - y, pointX - x) * 180.0f / 3.14159265f;
}


float Node::getAngleFromNode(Node* other) const
{
	return getAngleFromPoint(other->getX(), other->getY());
}

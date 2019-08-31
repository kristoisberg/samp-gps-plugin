#include <cmath>

#include "node.h"
#include <algorithm>


Node::Node(const int id, const float x, const float y, const float z)
{
	id_ = id;
	is_set_for_deletion_ = false;

	x_ = x;
	y_ = y;
	z_ = z;
}


void Node::setForDeletion()
{
	is_set_for_deletion_ = true;
}


bool Node::isSetForDeletion() const
{
	return is_set_for_deletion_;
}


void Node::addConnection(Connection* connection)
{
	connections_.push_back(connection);
}


void Node::removeConnection(Connection* connection)
{
	const auto it = std::find(connections_.begin(), connections_.end(), connection);

	if (it != connections_.end())
	{
		connections_.erase(it);
	}
}


std::vector<Connection*> Node::getConnections() const
{
	return connections_;
}


void Node::addToPath(Path* path, int index)
{
	paths_lock_.lock();
	paths_.emplace_back(path, index);
	paths_lock_.unlock();
}


int Node::getPositionInPath(Path* path)
{
	paths_lock_.lock();

	const auto it = std::find_if(paths_.begin(), paths_.end(), [&path](const std::pair<Path*, int>& pair) { return pair.first == path; });
	const auto result = (it == paths_.end()) ? -1 : it->second;

	paths_lock_.unlock();

	return result;
}


void Node::removeFromPath(Path* path)
{
	paths_lock_.lock();

	const auto it = std::find_if(paths_.begin(), paths_.end(), [&path](const std::pair<Path*, int>& pair) { return pair.first == path; });

	if (it != paths_.end())
	{
		paths_.erase(it);
	}

	paths_lock_.unlock();
}


int Node::getPathCount() const
{
	return paths_.size();
}


int Node::getID() const
{
	return id_;
}


float Node::getX() const
{
	return x_;
}


float Node::getY() const
{
	return y_;
}


float Node::getZ() const
{
	return z_;
}


float Node::getDistanceFromPoint(const float pointX, const float pointY, const float pointZ) const
{
	return sqrt(pow(pointX - x_, 2.0f) + pow(pointY - y_, 2.0f) + pow(pointZ - z_, 2.0f));
}


float Node::getDistanceFromNode(Node* other) const
{
	return getDistanceFromPoint(other->getX(), other->getY(), other->getZ());
}


float Node::getAngleFromPoint(const float pointX, const float pointY) const
{
	return atan2(pointY - y_, pointX - x_) * 180.0f / 3.14159265f;
}


float Node::getAngleFromNode(Node* other) const
{
	return getAngleFromPoint(other->getX(), other->getY());
}

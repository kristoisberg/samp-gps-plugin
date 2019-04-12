#include <cmath>

#include "node.h"


Node::Node(const int id, const float x, const float y, const float z) 
{
    this->id = id;

    this->x = x;
    this->y = y;
    this->z = z;
}


void Node::addConnection(Connection* connection) 
{
    this->connections.push_back(connection);
}


std::vector<Connection*> Node::getConnections() const
{
    return this->connections;
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

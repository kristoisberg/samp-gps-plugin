#include <cmath>

#include "node.h"


Node::Node(int id, float x, float y, float z) {
    this->id = id;

    this->x = x;
    this->y = y;
    this->z = z;
}


void Node::addConnection(Connection* connection) {
    this->connections.push_back(connection);
}


std::vector<Connection*> Node::getConnections() {
    return this->connections;
}


int Node::getID() {
    return this->id;
}


float Node::getX() {
    return this->x;
}


float Node::getY() {
    return this->y;
}


float Node::getZ() {
    return this->z;
}


float Node::getDistanceFromPoint(float pointX, float pointY, float pointZ) {
    return sqrt(pow(pointX - x, 2.0f) + pow(pointY - y, 2.0f) + pow(pointZ - z, 2.0f));
}


float Node::getDistanceFromNode(Node* other) {
    return getDistanceFromPoint(other->getX(), other->getY(), other->getZ());
}


float Node::getAngleFromPoint(float pointX, float pointY) {
    return atan2(pointY - y, pointX - x) * 180.0f / 3.14159265f;
}


float Node::getAngleFromNode(Node* other) {
    return getAngleFromPoint(other->getX(), other->getY());
}
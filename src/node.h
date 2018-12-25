#pragma once

#include <vector>

#include "common.h"


class Node {
public:
    Node(int id, float x, float y, float z);

    void addConnection(Connection* connection);
    std::vector<Connection*> getConnections();

    int getID();

    float getX();
    float getY();
    float getZ();

    float getDistanceFromPoint(float x, float y, float z);
    float getDistanceFromNode(Node* other);

    float getAngleFromPoint(float x, float y);
    float getAngleFromNode(Node* other);

private:
    int id = 0;

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    std::vector<Connection*> connections;
};
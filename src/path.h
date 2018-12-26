#pragma once

#include <deque>

#include "node.h"


class Path {
public:
    Path(float length);
    void addNodeToStart(Node* node);
    void addNodeToEnd(Node* node);
    float getLength();
    std::deque<Node*> getNodes();

private:
    std::deque<Node*> nodes;
    float length = 0.0f;
};
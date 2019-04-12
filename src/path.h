#pragma once

#include <deque>

#include "node.h"


class Path 
{
public:
    Path(float length);
	int getID() const;
	void setID(int id);
    void addNodeToStart(Node* node);
    void addNodeToEnd(Node* node);
    float getLength() const;
    std::deque<Node*> getNodes() const;

private:
	int id;
    std::deque<Node*> nodes;
    float length;
};
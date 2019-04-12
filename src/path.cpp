#include "path.h"


Path::Path(const float length) 
{
	this->id = INVALID_PATH_ID;
    this->length = length;
}


int Path::getID() const
{
	return this->id;
}


void Path::setID(const int id)
{
	this->id = id;
}


void Path::addNodeToStart(Node* node) 
{
    this->nodes.push_front(node);
}


void Path::addNodeToEnd(Node* node) 
{
    this->nodes.push_back(node);
}


float Path::getLength() const
{
    return this->length;
}


std::deque<Node*> Path::getNodes() const
{
    return this->nodes;
}

#include "path.h"


Path::Path(const float length)
{
	id_ = INVALID_PATH_ID;
	length_ = length;
}


int Path::getID() const
{
	return id_;
}


void Path::setID(const int id)
{
	id_ = id;
}


void Path::addNodeToStart(Node* node)
{
	nodes_.push_front(node);
}


void Path::addNodeToEnd(Node* node)
{
	nodes_.push_back(node);
}


float Path::getLength() const
{
	return length_;
}


std::deque<Node*> Path::getNodes() const
{
	return nodes_;
}

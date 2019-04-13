#include "connection.h"


Connection::Connection(Node* source, Node* target, const float distance)
{
	this->source = source;
	this->target = target;
	this->distance = distance;
}


Node* Connection::getSource() const
{
	return this->source;
}


Node* Connection::getTarget() const
{
	return this->target;
}


float Connection::getDistance() const
{
	return this->distance;
}
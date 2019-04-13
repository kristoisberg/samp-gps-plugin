#include "connection.h"


Connection::Connection(const int id, Node* source, Node* target, const float distance)
{
	this->id = id;
	this->source = source;
	this->target = target;
	this->distance = distance;
}


int Connection::getId() const
{
	return this->id;
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
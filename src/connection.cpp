#include "connection.h"


Connection::Connection(Node* target, const float distance)
{
	this->target = target;
	this->distance = distance;
}


float Connection::getDistance() const
{
	return this->distance;
}


Node* Connection::getTarget() const
{
	return this->target;
}
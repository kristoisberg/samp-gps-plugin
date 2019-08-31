#include "connection.h"


Connection::Connection(const int id, Node* source, Node* target, const float distance)
{
	id_ = id;
	source_ = source;
	target_ = target;
	distance_ = distance;
}


int Connection::getId() const
{
	return id_;
}


Node* Connection::getSource() const
{
	return source_;
}


Node* Connection::getTarget() const
{
	return target_;
}


float Connection::getDistance() const
{
	return distance_;
}
#pragma once

#include "common.h"


class Connection
{
public:
	Connection(Node* source, Node* target, float distance);

	Node* getSource() const;
	Node* getTarget() const;
	float getDistance() const;

private:
	Node* source;
	Node* target;
	float distance;
};
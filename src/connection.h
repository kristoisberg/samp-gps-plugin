#pragma once

#include "common.h"


class Connection 
{
public:
	Connection(Node* target, float distance);

	Node* getTarget() const;
	float getDistance() const;

private:
    Node* target;
    float distance;
};
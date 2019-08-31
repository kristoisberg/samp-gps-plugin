#pragma once

#include "common.h"


class Connection
{
public:
	Connection(int id, Node* source, Node* target, float distance);

	int getId() const;
	Node* getSource() const;
	Node* getTarget() const;
	float getDistance() const;

private:
	int id_;
	Node* source_;
	Node* target_;
	float distance_;
};
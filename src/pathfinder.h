#pragma once

#include "container.h"
#include "callback.h"


namespace Pathfinder
{
	Path* FindPathInternal(Node* start, Node* target);
	int FindPath(Node* start, Node* target);
	void FindPathThreaded(Node* start, Node* target, Callback* callback);
};
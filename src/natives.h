#pragma once

#include <iostream>

#include "pathfinder.h"


#define INVALID_NODE_ID -1
#define INVALID_PATH_ID -1


enum {
    GPS_ERROR_NONE              =  0,
    GPS_ERROR_INVALID_PARAMS    = -1,
    GPS_ERROR_INVALID_PATH      = -2,
    GPS_ERROR_INVALID_NODE      = -3,
    GPS_ERROR_INTERNAL          = -4
};


#define CHECK_PARAMS(n) \
	if (params[0] != (n * 4)) { \
		std::cout << __func__ << ": Expecting " << n << "parameter(s), but found " << (params[0] / sizeof(cell)) << ".\n"; \
		return GPS_ERROR_INVALID_PARAMS; \
	}


namespace Natives {
    cell AMX_NATIVE_CALL IsValidNode(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetNodePos(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetDistanceBetweenNodes(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetNodeDistanceFromPoint(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetClosestNodeToPoint(AMX* amx, cell* params);

	cell AMX_NATIVE_CALL FindPath(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL FindPathThreaded(AMX* amx, cell* params);

	cell AMX_NATIVE_CALL IsValidPath(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetPathSize(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetPathLength(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetPathNode(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL DestroyPath(AMX* amx, cell* params);
}
#pragma once

#include <iostream>

#include "pathfinder.h"


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
    cell AMX_NATIVE_CALL IsValidMapNode(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetMapNodePos(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetDistanceBetweenMapNodes(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetAngleBetweenMapNodes(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetMapNodeDistanceFromPoint(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetMapNodeAngleFromPoint(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetClosestMapNodeToPoint(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetMapNodeConnectionCount(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetHighestMapNodeID(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetRandomMapNode(AMX* amx, cell* params);

	cell AMX_NATIVE_CALL FindPath(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL FindPathThreaded(AMX* amx, cell* params);

	cell AMX_NATIVE_CALL IsValidPath(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetPathSize(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetPathLength(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetPathNode(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL DestroyPath(AMX* amx, cell* params);
}
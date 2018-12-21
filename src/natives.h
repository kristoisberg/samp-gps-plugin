#pragma once

#include "../lib/sdk/amx/amx.h"

#include "common.h"
#include "pathfinder.h"


#define CHECK_PARAMS(n) \
	if (params[0] != (n * 4)) { \
		logprintf("%s: Expecting %d parameter(s), but found %d.", __func__, n, params[0] / sizeof(cell)); \
		return 0; \
	}


namespace Natives {
    cell AMX_NATIVE_CALL IsValidNode(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetNodePos(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL FindPathFromNodeToNode(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL IsValidPath(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetPathSize(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetPathLength(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetPathNode(AMX* amx, cell* params);
}
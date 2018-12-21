#include <map>

#include "natives.h"


namespace Natives {
    cell AMX_NATIVE_CALL IsValidNode(AMX* amx, cell* params) {
        CHECK_PARAMS(1);

        int id = static_cast<int>(params[1]);

        return Pathfinder::GetNodeByID(id) != nullptr;
    }


    cell AMX_NATIVE_CALL GetNodePos(AMX* amx, cell* params) {
        CHECK_PARAMS(4);

        int id = static_cast<int>(params[1]);
        Pathfinder::Node* node = Pathfinder::GetNodeByID(id);

        if (node == nullptr) {
            return 0;
        }

        cell* addr = NULL;

        amx_GetAddr(amx, params[2], &addr);
        *addr = amx_ftoc(node->x);

        amx_GetAddr(amx, params[3], &addr);
        *addr = amx_ftoc(node->y);

        amx_GetAddr(amx, params[4], &addr);
        *addr = amx_ftoc(node->z);
        
        return 1;
    }


    cell AMX_NATIVE_CALL FindPathFromNodeToNode(AMX* amx, cell* params) {
        CHECK_PARAMS(2);

        int start_id = static_cast<int>(params[1]);
        Pathfinder::Node* start = Pathfinder::GetNodeByID(start_id);

        if (start == nullptr) {
            return -1;
        }

        int target_id = static_cast<int>(params[2]);
        Pathfinder::Node* target = Pathfinder::GetNodeByID(target_id);

        if (target == nullptr) {
            return -1;
        }

        return Pathfinder::FindPathFromNodeToNode(start, target);
    }


    cell AMX_NATIVE_CALL IsValidPath(AMX* amx, cell* params) {
        CHECK_PARAMS(1);

        int id = static_cast<int>(params[1]);

        return Pathfinder::GetPathByID(id) != nullptr;
    }


    cell AMX_NATIVE_CALL GetPathSize(AMX* amx, cell* params) {
        CHECK_PARAMS(1);

        int id = static_cast<int>(params[1]);
        Pathfinder::Path* path = Pathfinder::GetPathByID(id);

        if (path == nullptr) {
            return 0;
        }

        return path->nodes.size();
    }


    cell AMX_NATIVE_CALL GetPathLength(AMX* amx, cell* params) {
        CHECK_PARAMS(1);

        int id = static_cast<int>(params[1]);
        Pathfinder::Path* path = Pathfinder::GetPathByID(id);
        float result = 0.0f;

        if (path != nullptr) {
            result = path->length;
        }

        return amx_ftoc(result);
    }


    cell AMX_NATIVE_CALL GetPathNode(AMX* amx, cell* params) {
        CHECK_PARAMS(2);

        int id = static_cast<int>(params[1]);
        Pathfinder::Path* path = Pathfinder::GetPathByID(id);

        if (path == nullptr) {
            return -1;
        }

        int index = static_cast<int>(params[2]);

        if (index < 0 || index >= path->nodes.size()) {
            return -1;
        }

        return path->nodes.at(index)->id;
    }
}
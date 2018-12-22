#include <map>
#include <thread>
#include <iostream>

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
            return GPS_ERROR_INVALID_NODE;
        }

        cell* addr = NULL;

        amx_GetAddr(amx, params[2], &addr);
        *addr = amx_ftoc(node->x);

        amx_GetAddr(amx, params[3], &addr);
        *addr = amx_ftoc(node->y);

        amx_GetAddr(amx, params[4], &addr);
        *addr = amx_ftoc(node->z);
        
        return GPS_ERROR_NONE;
    }

    
    cell AMX_NATIVE_CALL GetDistanceBetweenNodes(AMX* amx, cell* params) {
        CHECK_PARAMS(3);

        int first_id = static_cast<int>(params[1]);
        Pathfinder::Node* first = Pathfinder::GetNodeByID(first_id);

        if (first == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        int second_id = static_cast<int>(params[2]);
        Pathfinder::Node* second = Pathfinder::GetNodeByID(second_id);

        if (second == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        float distance = Pathfinder::GetDistanceBetweenNodes(first, second);

        cell* addr = NULL;
        amx_GetAddr(amx, params[3], &addr);
        *addr = amx_ftoc(distance);

        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL GetNodeDistanceFromPoint(AMX* amx, cell* params) {
        CHECK_PARAMS(4);

        int id = static_cast<int>(params[1]);
        Pathfinder::Node* node = Pathfinder::GetNodeByID(id);

        if (node == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        float x = amx_ctof(params[2]), y = amx_ctof(params[3]), z = amx_ctof(params[4]);

        float distance = Pathfinder::GetNodeDistanceFromPoint(node, x, y, z);

        cell* addr = NULL;
        amx_GetAddr(amx, params[5], &addr);
        *addr = amx_ftoc(distance);
     
        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL GetClosestNodeToPoint(AMX* amx, cell* params) {
        CHECK_PARAMS(5);

        float x = amx_ctof(params[1]), y = amx_ctof(params[2]), z = amx_ctof(params[3]);

        int ignoredid = static_cast<int>(params[5]);
        Pathfinder::Node* ignored = nullptr;

        if (ignoredid != INVALID_NODE_ID) {
            ignored = Pathfinder::GetNodeByID(ignoredid);

            if (ignored == nullptr) {
                return GPS_ERROR_INVALID_NODE;
            }
        }

        Pathfinder::Node* result;
        float distance = std::numeric_limits<float>::infinity(), temp = 0.0f;

        for (std::pair<int, Pathfinder::Node> node : Pathfinder::nodes) {
            if (ignored == &node.second) {
                continue;
            }

            temp = Pathfinder::GetNodeDistanceFromPoint(&node.second, x, y, z);

            if (temp < distance) {
                result = &node.second;
                distance = temp;
            }
        }

        cell* addr = NULL;
        amx_GetAddr(amx, params[4], &addr);

        if (result == nullptr) {
            *addr = INVALID_NODE_ID;
        } else {
            *addr = result->id;
        }   
     
        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL FindPath(AMX* amx, cell* params) {
        CHECK_PARAMS(3);

        int start_id = static_cast<int>(params[1]);
        Pathfinder::Node* start = Pathfinder::GetNodeByID(start_id);

        if (start == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        int target_id = static_cast<int>(params[2]);
        Pathfinder::Node* target = Pathfinder::GetNodeByID(target_id);

        if (target == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        cell* addr = NULL;
        amx_GetAddr(amx, params[3], &addr);
        *addr = Pathfinder::FindPath(start, target);

        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL FindPathThreaded(AMX* amx, cell* params) {
        CHECK_PARAMS(4);

        int start_id = static_cast<int>(params[1]);
        Pathfinder::Node* start = Pathfinder::GetNodeByID(start_id);

        if (start == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        int target_id = static_cast<int>(params[2]);
        Pathfinder::Node* target = Pathfinder::GetNodeByID(target_id);

        if (target == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        std::string callback = amx_GetCppString(amx, params[3]);
        int id = static_cast<int>(params[4]);

        try {
            std::thread t(Pathfinder::FindPathThreaded, amx, start, target, callback, id);
            t.detach();
        } catch (std::exception e) {
            std::cout << "Failed to dispatch thread: " << e.what() << "\n";
            return GPS_ERROR_INTERNAL;
        }

        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL IsValidPath(AMX* amx, cell* params) {
        CHECK_PARAMS(1);

        int id = static_cast<int>(params[1]);

        return Pathfinder::GetPathByID(id) != nullptr;
    }


    cell AMX_NATIVE_CALL GetPathSize(AMX* amx, cell* params) {
        CHECK_PARAMS(2);

        int id = static_cast<int>(params[1]); 
        Pathfinder::Path* path = Pathfinder::GetPathByID(id);

        if (path == nullptr) {
            return GPS_ERROR_INVALID_PATH;
        }

        cell* addr = NULL;
        amx_GetAddr(amx, params[2], &addr);
        *addr = path->nodes.size();

        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL GetPathLength(AMX* amx, cell* params) {
        CHECK_PARAMS(2);

        int id = static_cast<int>(params[1]);
        Pathfinder::Path* path = Pathfinder::GetPathByID(id);

        if (path == nullptr) {
            return GPS_ERROR_INVALID_PATH;
        }

        float result = path->length;

        cell* addr = NULL;
        amx_GetAddr(amx, params[2], &addr);
        *addr = amx_ftoc(path->length);

        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL GetPathNode(AMX* amx, cell* params) {
        CHECK_PARAMS(3);

        int id = static_cast<int>(params[1]);
        Pathfinder::Path* path = Pathfinder::GetPathByID(id);

        if (path == nullptr) {
            return GPS_ERROR_INVALID_PATH;
        }

        int index = static_cast<int>(params[2]);

        if (index < 0 || index >= path->nodes.size()) {
            return GPS_ERROR_INVALID_NODE;
        }

        cell* addr = NULL;
        amx_GetAddr(amx, params[3], &addr);
        *addr = path->nodes.at(index)->id;

        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL DestroyPath(AMX* amx, cell* params) {
        CHECK_PARAMS(1);

        int id = static_cast<int>(params[1]);
        Pathfinder::Path* path = Pathfinder::GetPathByID(id);

        if (path == nullptr) {
            return GPS_ERROR_INVALID_PATH;
        }

        Pathfinder::paths.erase(id);

        return GPS_ERROR_NONE;
    }
}
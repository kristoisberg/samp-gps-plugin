#include <map>
#include <thread>
#include <iostream>
#include <random>
#include <algorithm>

#include "natives.h"
#include "callback.h"
#include "node.h"


namespace Natives {
    std::mt19937 mersenne{std::random_device{}()};


    cell AMX_NATIVE_CALL IsValidMapNode(AMX* amx, cell* params) {
        CHECK_PARAMS(1);

        int id = static_cast<int>(params[1]);

        return Pathfinder::GetNodeByID(id) != nullptr;
    }


    cell AMX_NATIVE_CALL GetHighestMapNodeID(AMX* amx, cell* params) {
        return Pathfinder::highest_node_id;
    }


    cell AMX_NATIVE_CALL GetRandomMapNode(AMX* amx, cell* params) {
        CHECK_PARAMS(1);

        if (Pathfinder::nodes.empty()) {
            return GPS_ERROR_INVALID_NODE;
        }

        std::vector<std::pair<int, Node>> result;

        std::sample(Pathfinder::nodes.begin(), Pathfinder::nodes.end(), std::back_inserter(result), 1, mersenne);

        cell* addr = NULL;
        amx_GetAddr(amx, params[1], &addr);
        *addr = result.at(0).second.getID();

        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL GetMapNodePos(AMX* amx, cell* params) {
        CHECK_PARAMS(4);

        int id = static_cast<int>(params[1]);
        Node* node = Pathfinder::GetNodeByID(id);

        if (node == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        float x = node->getX(), y = node->getY(), z = node->getZ();

        cell* addr = NULL;

        amx_GetAddr(amx, params[2], &addr);
        *addr = amx_ftoc(x);

        amx_GetAddr(amx, params[3], &addr);
        *addr = amx_ftoc(y);

        amx_GetAddr(amx, params[4], &addr);
        *addr = amx_ftoc(z);
        
        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL GetMapNodeConnectionCount(AMX* amx, cell* params) {
        CHECK_PARAMS(2);

        int id = static_cast<int>(params[1]);
        Node* node = Pathfinder::GetNodeByID(id);

        if (node == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        cell* addr = NULL;
        amx_GetAddr(amx, params[2], &addr);
        *addr = node->getConnections().size();

        return GPS_ERROR_NONE;
    }

    
    cell AMX_NATIVE_CALL GetDistanceBetweenMapNodes(AMX* amx, cell* params) {
        CHECK_PARAMS(3);

        int first_id = static_cast<int>(params[1]);
        Node* first = Pathfinder::GetNodeByID(first_id);

        if (first == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        int second_id = static_cast<int>(params[2]);
        Node* second = Pathfinder::GetNodeByID(second_id);

        if (second == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        float distance = first->getDistanceFromNode(second);
     
        cell* addr = NULL;
        amx_GetAddr(amx, params[3], &addr);
        *addr = amx_ftoc(distance);

        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL GetMapNodeDistanceFromPoint(AMX* amx, cell* params) {
        CHECK_PARAMS(5);

        int id = static_cast<int>(params[1]);
        Node* node = Pathfinder::GetNodeByID(id);

        if (node == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        float x = amx_ctof(params[2]), y = amx_ctof(params[3]), z = amx_ctof(params[4]);

        float distance = node->getDistanceFromPoint(x, y, z);

        cell* addr = NULL;
        amx_GetAddr(amx, params[5], &addr);
        *addr = amx_ftoc(distance);
     
        return GPS_ERROR_NONE;
    }

    
    cell AMX_NATIVE_CALL GetAngleBetweenMapNodes(AMX* amx, cell* params) {
        CHECK_PARAMS(3);

        int first_id = static_cast<int>(params[1]);
        Node* first = Pathfinder::GetNodeByID(first_id);

        if (first == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        int second_id = static_cast<int>(params[2]);
        Node* second = Pathfinder::GetNodeByID(second_id);

        if (second == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        float angle = first->getAngleFromNode(second);
     
        cell* addr = NULL;
        amx_GetAddr(amx, params[3], &addr);
        *addr = amx_ftoc(angle);

        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL GetMapNodeAngleFromPoint(AMX* amx, cell* params) {
        CHECK_PARAMS(4);

        int id = static_cast<int>(params[1]);
        Node* node = Pathfinder::GetNodeByID(id);

        if (node == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        float x = amx_ctof(params[2]), y = amx_ctof(params[3]);

        float angle = node->getAngleFromPoint(x, y);

        cell* addr = NULL;
        amx_GetAddr(amx, params[4], &addr);
        *addr = amx_ftoc(angle);
     
        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL GetClosestMapNodeToPoint(AMX* amx, cell* params) {
        CHECK_PARAMS(5);

        float x = amx_ctof(params[1]), y = amx_ctof(params[2]), z = amx_ctof(params[3]);
        int ignored = static_cast<int>(params[5]);
        int result = INVALID_NODE_ID;
        float distance = std::numeric_limits<float>::infinity(), temp = 0.0f;
        int id = INVALID_NODE_ID;

        for (std::pair<int, Node> node : Pathfinder::nodes) {
            id = node.second.getID();

            if (ignored == id) {
                continue;
            }

            temp = node.second.getDistanceFromPoint(x, y, z);

            if (temp < distance) {
                result = id;
                distance = temp;
            }
        }

        cell* addr = NULL;
        amx_GetAddr(amx, params[4], &addr);
        *addr = result; 
     
        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL FindPath(AMX* amx, cell* params) {
        CHECK_PARAMS(3);

        int start_id = static_cast<int>(params[1]);
        Node* start = Pathfinder::GetNodeByID(start_id);

        if (start == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        int target_id = static_cast<int>(params[2]);
        Node* target = Pathfinder::GetNodeByID(target_id);

        if (target == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        int result = Pathfinder::FindPath(start, target);

        if (result == -1) {
            return GPS_ERROR_INVALID_PATH;
        }

        cell* addr = NULL;
        amx_GetAddr(amx, params[3], &addr);
        *addr = result;

        return GPS_ERROR_NONE;
    }


    cell AMX_NATIVE_CALL FindPathThreaded(AMX* amx, cell* params) {
        if (params[0] < (3 * 4)) { 
            std::cout << "FindPathThreaded: Expecting at least 3 parameter(s), but found " << (params[0] / sizeof(cell)) << ".\n"; 
            return GPS_ERROR_INVALID_PARAMS; 
        }

        int start_id = static_cast<int>(params[1]);
        Node* start = Pathfinder::GetNodeByID(start_id);

        if (start == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        int target_id = static_cast<int>(params[2]);
        Node* target = Pathfinder::GetNodeByID(target_id);

        if (target == nullptr) {
            return GPS_ERROR_INVALID_NODE;
        }

        std::string callback_name = amx_GetCppString(amx, params[3]);

        char* format = NULL;
        amx_StrParam(amx, params[4], format);

        Callback* callback = new Callback(amx, callback_name, format, params, 4);

        try { 
            std::thread t(Pathfinder::FindPathThreaded, start, target, callback);
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

        unsigned int index = static_cast<unsigned int>(params[2]);

        if (index < 0 || index >= path->nodes.size()) {
            return GPS_ERROR_INVALID_NODE;
        }

        cell* addr = NULL;
        amx_GetAddr(amx, params[3], &addr);
        *addr = path->nodes.at(index)->getID();

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
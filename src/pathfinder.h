#pragma once

#include <vector>
#include <unordered_map>
#include <limits>

#include "../lib/sdk/amx/amx2.h"


namespace Pathfinder {
    extern float infinity;

    struct Connection; 

    struct Node {
        float x = 0.0f, y = 0.0f, z = 0.0f;
        int id = 0;
        std::vector<Connection> connections;
    };

    struct Connection {
        Node* target;
        float distance = 0.0f;
    };

    struct ShortestPathTo {
        Node* node;
        ShortestPathTo* previous;
        float distance = infinity;
        float target_distance = infinity;

        friend bool operator<(const ShortestPathTo& lhs, const ShortestPathTo& rhs) {
            return lhs.distance + lhs.target_distance < rhs.distance + rhs.target_distance;
        };
    };

    struct Path {
        std::vector<Node*> nodes;
        float length = 0.0f;
    };


    extern std::unordered_map<int, Node> nodes;

    extern std::unordered_map<int, Pathfinder::Path> paths;
    extern int path_count;


    bool AddNode(int id, float x, float y, float z);
    bool AddConnection(int startid, int targetid);

    Node* GetNodeByID(int id);
    float GetNodeDistanceFromPoint(Node* node, float x, float y, float z);
    float GetDistanceBetweenNodes(Node* first, Node* second);

    int FindPath(Node* start, Node* target);
    void FindPathThreaded(AMX* amx, Node* start, Node* target, std::string callback, int id);

    Path* GetPathByID(int id);
};
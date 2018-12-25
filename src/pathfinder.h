#pragma once

#include <vector>
#include <unordered_map>
#include <limits>

#include "common.h"
#include "node.h"
#include "connection.h"
#include "callback.h"


namespace Pathfinder {
    extern float infinity;

    struct ShortestPathTo {
        Node* node;
        ShortestPathTo* previous;
        float distance = infinity;
        float target_distance = infinity;
        float total_distance = infinity;

        friend bool operator<(const ShortestPathTo& lhs, const ShortestPathTo& rhs) {
            return lhs.total_distance < rhs.total_distance;
        };
    };

    struct Path {
        std::vector<Node*> nodes;
        float length = 0.0f;
    };


    extern std::unordered_map<int, Node> nodes;
    extern int highest_node_id;

    extern std::unordered_map<int, Path> paths;
    extern int path_count;


    bool AddNode(int id, float x, float y, float z);
    bool AddConnection(int startid, int targetid);

    Node* GetNodeByID(int id);

    Path* FindPathInternal(Node* start, Node* target);
    int FindPath(Node* start, Node* target);
    void FindPathThreaded(Node* start, Node* target, Callback* callback);

    Path* GetPathByID(int id);
};
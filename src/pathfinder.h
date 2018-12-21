#pragma once

#include <vector>
#include <map>
#include <limits>


namespace Pathfinder {
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
        float distance = std::numeric_limits<float>::infinity();

        friend bool operator<(const ShortestPathTo& lhs, const ShortestPathTo& rhs) {
            return lhs.distance < rhs.distance;
        };
    };

    struct Path {
        std::vector<Node*> nodes;
        float length = 0.0f;
    };


    extern std::map<int, Node> nodes;

    extern std::map<int, Pathfinder::Path> paths;
    extern int path_count;


    bool AddNode(int id, float x, float y, float z);
    bool AddConnection(int startid, int targetid);
    Node* GetNodeByID(int id);
    int FindPathFromNodeToNode(Node* start, Node* target);
    Path* GetPathByID(int id);
};
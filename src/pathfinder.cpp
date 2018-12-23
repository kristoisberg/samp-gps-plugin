#include <cmath>
#include <set>
#include <queue>
#include <iostream>

#include "pathfinder.h"
#include "amx.h"


namespace Pathfinder {
    float infinity = std::numeric_limits<float>::infinity();

    std::unordered_map<int, Node> nodes;
    int largest_node_id = -1;

    std::unordered_map<int, Pathfinder::Path> paths;
    int path_count = 0;


    bool AddNode(int id, float x, float y, float z) {
        if (GetNodeByID(id) != nullptr) {
            return false;
        }

        nodes.insert({id, Node{x, y, z, id}});

        if (id > largest_node_id) {
            largest_node_id = id;
        }

        return true;
    } 


    bool AddConnection(int startid, int targetid) {
        Node* start = GetNodeByID(startid);

        if (start == nullptr) {
            return false;
        }

        Node* target = GetNodeByID(targetid);

        if (target == nullptr) {
            return false;
        }

        start->connections.push_back(Connection{target, GetDistanceBetweenNodes(start, target)});
        return true;
    }


    Node* GetNodeByID(int id) {
        auto node = nodes.find(id);

        if (node != nodes.end()) {
            return &node->second;
        } else {
            return nullptr;
        }
    }


    float GetNodeDistanceFromPoint(Node* node, float x, float y, float z) {
        return sqrt(pow(node->x - x, 2.0f) + pow(node->y - y, 2.0f) + pow(node->z - z, 2.0f));
    }


    float GetDistanceBetweenNodes(Node* first, Node* second) {
        return GetNodeDistanceFromPoint(first, second->x, second->y, second->z);
    }


    Path* FindPathInternal(Node* start, Node* target) {
        if (start == target) {
            return new Path{{start}, 0.0f};
        }

        std::priority_queue<ShortestPathTo*> queue;
        std::vector<ShortestPathTo> existing_paths(largest_node_id + 1);
        ShortestPathTo* path = nullptr;
        ShortestPathTo* next_path = nullptr;
        ShortestPathTo* solution = nullptr;
        float distance = 0.0f;

        path = &existing_paths.at(start->id);
        path->distance = 0.0f;
        path->target_distance = GetDistanceBetweenNodes(start, target);
        path->previous = nullptr;
        path->node = start;
        
        queue.push(path);

        while (!queue.empty()) {
            path = queue.top();
            queue.pop();

            if (solution != nullptr && solution < path) {
                continue;
            }

            for (Connection const& connection : path->node->connections) {
                next_path = &existing_paths.at(connection.target->id);
                distance = path->distance + connection.distance;

                if (next_path->distance > distance) {
                    if (next_path->distance == infinity) {
                        next_path->node = connection.target;
                        next_path->target_distance = GetDistanceBetweenNodes(connection.target, target);

                        if (connection.target == target) {
                            solution = next_path;
                        }
                    }

                    next_path->distance = distance;
                    next_path->previous = path;

                    queue.push(next_path);
                }
            }
        }

        if (solution == nullptr) {
            return nullptr;
        }

        Path* result = new Path();
        result->length = solution->distance;

        while (solution != nullptr) {
            result->nodes.insert(result->nodes.begin(), solution->node);

            solution = solution->previous;
        }

        return result;
    }


    int FindPath(Node* start, Node* target) {
        Path* path = FindPathInternal(start, target);

        if (path == nullptr) {
            return -1;
        }

        int id = path_count++;
        paths[id] = *path;
        return id;
    }


    void FindPathThreaded(Node* start, Node* target, Callback* callback) {
        Path* path = FindPathInternal(start, target);

        if (path == nullptr) {
            return;
        }

        int id = path_count++;
        paths[id] = *path;

        callback->setResult(id);
        amx::QueueCallback(callback->getAMX(), callback);
    }


    Path* GetPathByID(int id) {
        auto path = paths.find(id);

        if (path != paths.end()) {
            return &path->second;
        } else {
            return nullptr;
        }
    }
};
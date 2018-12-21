#include <cmath>
#include <map>
#include <queue>
#include <iostream>
#include <functional>

#include "pathfinder.h"
#include "amx.h"


namespace Pathfinder {
    std::map<int, Node> nodes;

    std::map<int, Pathfinder::Path> paths;
    int path_count = 0;


    bool AddNode(int id, float x, float y, float z) {
        if (GetNodeByID(id) != nullptr) {
            return false;
        }

        nodes.insert({id, Node{x, y, z, id}});
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

        start->connections.push_back(Connection{target, sqrt(pow(start->x - target->x, 2.0f) + pow(start->y - target->y, 2.0f) + pow(start->z - target->z, 2.0f))});
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


    Path* GetPathByID(int id) {
        auto path = paths.find(id);

        if (path != paths.end()) {
            return &path->second;
        } else {
            return nullptr;
        }
    }


    Path* FindPath(Node* start, Node* target) {
        if (start == target) {
            return new Path{{start}, 0.0f};
        }

        std::priority_queue<ShortestPathTo*> queue;
        std::map<Node*, ShortestPathTo*> existing_paths;
        ShortestPathTo* path = nullptr;
        ShortestPathTo* next_path = nullptr;
        ShortestPathTo* solution = nullptr;
        float distance = 0.0f;

        queue.push(new ShortestPathTo{start, nullptr, 0.0f});

        while (!queue.empty()) {
            path = queue.top();
            queue.pop();

            for (Connection const& connection : path->node->connections) {
                auto it = existing_paths.find(connection.target);
                distance = path->distance + connection.distance;

                if (it == existing_paths.end()) {
                    next_path = new ShortestPathTo{connection.target, path, distance};
                    queue.push(next_path);
                    existing_paths[connection.target] = next_path;

                    if (connection.target == target) {
                        solution = next_path;
                    }
                } else {
                    next_path = it->second;
                    
                    if (next_path->distance > distance) {
                        next_path->distance = distance;
                        next_path->previous = path;
                        queue.push(next_path);
                    }
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


    int FindPathFromNodeToNode(Node* start, Node* target) {
        Path* path = FindPath(start, target);

        if (path == nullptr) {
            return -1;
        }

        int id = path_count++;
        paths[id] = *path;
        return id;
    }


    void FindPathFromNodeToNodeThreaded(AMX* amx, Node* start, Node* target, std::string callback, int extra_id) {
        Path* path = FindPath(start, target);

        if (path == nullptr) {
            return;
        }

        int id = path_count++;
        paths[id] = *path;

        amx_list[amx].callback_queue.push_back({id, callback, extra_id});
    }
};
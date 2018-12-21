#include <cmath>
#include <map>
#include <queue>
#include <iostream>
#include <functional>

#include "pathfinder.h"


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


    int FindPathFromNodeToNode(Node* start, Node* target) {
        if (start == target) {
            int id = path_count++;
            paths[id] = Path{{start}, 0.0f};
            return id;
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
            return -1;
        }

        Path result = Path();

        while (solution != nullptr) {
            result.nodes.insert(result.nodes.begin(), solution->node);

            solution = solution->previous;
        }

        int id = path_count++;
        paths[id] = result;
        return id;
    }


    /*int FindPathFromNodeToNode(Node* start, Node* target) {
        std::priority_queue<ShortestPathTo*, std::vector<ShortestPathTo*>, std::function<bool(ShortestPathTo*, ShortestPathTo*)>> queue(ComparePaths);
        std::map<Node*, ShortestPathTo*> existing_paths;

        

        ShortestPathTo* solution = nullptr;
        ShortestPathTo* path = nullptr;
        ShortestPathTo* next_path = nullptr;
        float solution_distance = 0.0f, path_distance = 0.0f, current_distance = 0.0f;

        while (!queue.empty()) {
            path = queue.top();
            current_distance = GetPathLength(path);
            queue.pop();

            for (Connection const& connection : path->node->connections) {
                path_distance = current_distance + connection.distance;

                std::cout << path->node->id << " > " << connection.target->id << ": " << path_distance << "\n";

                if (solution != nullptr && path_distance > solution_distance) {
                    continue;
                }

                auto it = existing_paths.find(connection.target);

                if (it != existing_paths.end()) {
                    next_path = it->second;

                    if (GetPathLength(next_path) > path_distance) {
                        next_path->previous = path;

                        if (next_path == solution) {
                            solution_distance = path_distance;
                        }
                    }
                } else {
                    next_path = new ShortestPathTo{connection.target, path};

                    existing_paths[connection.target] = next_path;

                    if (connection.target == target) {
                        solution = next_path;
                        solution_distance = path_distance;
                    }

                    queue.push(next_path);
                }
            }
        }

    }*/
};
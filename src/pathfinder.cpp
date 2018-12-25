#include <cmath>
#include <set>
#include <queue>
#include <iostream>

#include "pathfinder.h"
#include "amx.h"


namespace Pathfinder {
    float infinity = std::numeric_limits<float>::infinity();

    std::unordered_map<int, Node> nodes;
    int highest_node_id = -1;

    std::unordered_map<int, Path> paths;
    int path_count = 0;


    bool AddNode(int id, float x, float y, float z) {
        if (GetNodeByID(id) != nullptr) {
            return false;
        }

        nodes.insert({id, Node{id, x, y, z}});

        if (id > highest_node_id) {
            highest_node_id = id;
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

        start->addConnection(new Connection{target, start->getDistanceFromNode(target)});
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


    Path* FindPathInternal(Node* start, Node* target) {
        if (start == target) {
            return new Path{{start}, 0.0f};
        }

        std::priority_queue<ShortestPathTo*> queue;
        std::vector<ShortestPathTo> existing_paths(highest_node_id + 1);
        ShortestPathTo* path = nullptr;
        ShortestPathTo* next_path = nullptr;
        ShortestPathTo* solution = nullptr;
        float distance = 0.0f;

        path = &existing_paths.at(start->getID());
        path->distance = 0.0f;
        path->total_distance = path->target_distance = start->getDistanceFromNode(target);
        path->previous = nullptr;
        path->node = start;
        
        queue.push(path);

        while (!queue.empty()) {
            path = queue.top();
            queue.pop();

            if (solution != nullptr && solution < path) {
                continue;
            }

            for (Connection* connection : path->node->getConnections()) {
                next_path = &existing_paths.at(connection->target->getID());
                distance = path->distance + connection->distance;

                if (next_path->distance > distance) {
                    if (next_path->distance == infinity) {
                        next_path->node = connection->target;
                        next_path->target_distance = target->getDistanceFromNode(connection->target);

                        if (connection->target == target) {
                            solution = next_path;
                        }
                    }

                    next_path->distance = distance;
                    next_path->previous = path;
                    next_path->total_distance = next_path->target_distance + distance;
                    
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
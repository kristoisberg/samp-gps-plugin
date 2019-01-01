#include <cmath>
#include <set>
#include <queue>
#include <iostream>
#include <exception>

#include "pathfinder.h"
#include "amx.h"
 

namespace Pathfinder {
    float infinity = std::numeric_limits<float>::infinity();

    struct VisitedNode {
        Node* node;
        VisitedNode* previous;
        float distance = infinity;
        float target_distance = infinity;
        float total_distance = infinity;

        friend bool operator<(const VisitedNode& lhs, const VisitedNode& rhs) {
            return lhs.total_distance < rhs.total_distance;
        };
    };

    struct NodeComparison {
        bool operator() (const VisitedNode* lhs, const VisitedNode* rhs) const {
            return lhs->total_distance > rhs->total_distance;
        }
    };

    std::unordered_map<int, Node*> nodes;
    int highest_node_id = -1;

    std::unordered_map<int, Path*> paths;
    int path_count = 0;


    struct PathNotFoundException : public std::exception {
        const char* what() const throw() {
            return "Failed to find a path!";
        }
    };


    bool AddNode(int id, float x, float y, float z) {
        if (GetNodeByID(id) != nullptr) {
            return false;
        }

        nodes[id] = new Node(id, x, y, z);

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
            return node->second;
        } else {
            return nullptr;
        }
    }


    Path* FindPathInternal(Node* start, Node* target) {
        if (start == target) {
            Path* result = new Path(0.0f);
            result->addNodeToEnd(start);
            return result;
        }

        std::priority_queue<VisitedNode*, std::vector<VisitedNode*>, NodeComparison> queue;
        std::vector<VisitedNode> existing_paths(highest_node_id + 1);
        VisitedNode* path = nullptr; 
        VisitedNode* next_path = nullptr;
        VisitedNode* solution = nullptr;
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

            if (solution != nullptr && *solution < *path) {
                break;
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
            throw PathNotFoundException();
        }

        Path* result = new Path(solution->distance);

        while (solution != nullptr) {
            result->addNodeToStart(solution->node);
            solution = solution->previous;
        }

        return result;
    }


    int FindPath(Node* start, Node* target) {
        try {
            Path* path = FindPathInternal(start, target);

            int id = path_count++;
            paths[id] = path;
            return id;
        } catch (PathNotFoundException e) {
            return INVALID_PATH_ID;
        }
    }


    void FindPathThreaded(Node* start, Node* target, Callback* callback) {
        try {
            Path* path = FindPathInternal(start, target);

            int id = path_count++;
            paths[id] = path;

            callback->setResult(id);
            amx::QueueCallback(callback->getAMX(), callback);
        } catch (PathNotFoundException e) {
            callback->setResult(INVALID_PATH_ID);
            amx::QueueCallback(callback->getAMX(), callback);
        }    
    }


    Path* GetPathByID(int id) {
        auto path = paths.find(id);

        if (path != paths.end()) {
            return path->second;
        } else {
            return nullptr;
        }
    }
};
#include "path.h"


Path::Path(float length) {
    this->length = length;
}


void Path::addNodeToStart(Node* node) {
    this->nodes.push_front(node);
}


void Path::addNodeToEnd(Node* node) {
    this->nodes.push_back(node);
}


float Path::getLength() {
    return this->length;
}


std::deque<Node*> Path::getNodes() {
    return this->nodes;
}
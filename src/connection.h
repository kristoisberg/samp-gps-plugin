#pragma once

#include "node.h"


struct Connection {
    Node* target;
    float distance = 0.0f;
};
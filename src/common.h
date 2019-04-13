#pragma once

#include "../lib/sdk/amx/amx2.h"
#include "../lib/sdk/plugincommon.h"


const int INVALID_NODE_ID = -1;
const int INVALID_PATH_ID = -1;
const int INVALID_CONNECTION_ID = -1;


typedef void(*logprintf_t)(const char* format, ...);
extern logprintf_t logprintf;
extern void *pAMXFunctions;

class Path;
class Node;
class Connection;
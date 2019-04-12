#pragma once

#include "../lib/sdk/amx/amx2.h"
#include "../lib/sdk/plugincommon.h"


#define INVALID_NODE_ID -1
#define INVALID_PATH_ID -1


typedef void(*logprintf_t)(const char* format, ...);
extern logprintf_t logprintf;
extern void *pAMXFunctions;

class Node;
class Connection;
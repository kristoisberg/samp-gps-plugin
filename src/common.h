#pragma once

#include "../lib/sdk/amx/amx2.h"
#include "../lib/sdk/plugincommon.h"


typedef void (*logprintf_t)(const char* format, ...);
extern logprintf_t logprintf;
extern void *pAMXFunctions;


struct Path;
#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "../lib/sdk/amx/amx2.h"

#include "common.h"


struct AMXInfo {
	std::vector<Callback> callback_queue;
};

extern std::unordered_map<AMX*, AMXInfo> amx_list;


namespace amx {
	void load(AMX *amx);
	void unload(AMX *amx);
	void processTick(AMX *amx);
}

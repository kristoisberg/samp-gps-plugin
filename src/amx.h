#pragma once

#include <vector>
#include <unordered_map>
#include <mutex>

#include "common.h"
#include "callback.h"


struct AMXInfo 
{
	std::vector<Callback*> callback_queue;
};

extern std::unordered_map<AMX*, AMXInfo> amx_list;
extern std::mutex amx_list_lock;


namespace amx 
{
	void Load(AMX* amx);
	void Unload(AMX* amx);
	void ProcessTick(AMX* amx);
	void QueueCallback(AMX* amx, Callback* callback);
}

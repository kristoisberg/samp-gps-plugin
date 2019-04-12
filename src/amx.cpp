#include "common.h"
#include "amx.h"


std::unordered_map<AMX*, AMXInfo> amx_list;
std::mutex amx_list_lock;


namespace amx 
{
	void Load(AMX* amx) 
	{
		amx_list[amx] = AMXInfo();
	}


	void Unload(AMX* amx) 
	{
		amx_list.erase(amx);
	}


	void ProcessTick(AMX* amx) 
	{
		if (amx_list_lock.try_lock()) 
		{
			auto info = &amx_list[amx];

			for (auto callback : info->callback_queue) 
			{
				callback->call();
			}

			info->callback_queue.clear();
			amx_list_lock.unlock();
		}
	}


	void QueueCallback(AMX* amx, Callback* callback) 
	{
		amx_list_lock.lock();
        amx_list[amx].callback_queue.push_back(callback);
        amx_list_lock.unlock();
	}
}

#include <unordered_map>
#include <mutex>

#include "amx.h"


namespace AmxHandler
{
	namespace
	{
		std::unordered_map<AMX*, Amx*> amx_list;
	}


	void LoadAmx(AMX* amx)
	{
		amx_list[amx] = new Amx(amx);
	}


	void UnloadAmx(AMX* amx)
	{
		amx_list.erase(amx);
	}


	Amx* GetAmx(AMX* amx)
	{
		return amx_list.at(amx);
	}


	void ProcessTick()
	{
		for (auto amx : amx_list)
		{
			amx.second->processCallbacks();
		}
	}
}

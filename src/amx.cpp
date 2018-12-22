#include <iostream>

#include "common.h"
#include "amx.h"


std::unordered_map<AMX*, AMXInfo> amx_list;


namespace amx {
	void load(AMX *amx) {
		amx_list[amx] = AMXInfo();
	}


	void unload(AMX *amx) {
		amx_list.erase(amx);
	}


	void processTick(AMX *amx) {
		AMXInfo* info = &amx_list[amx];
		int error = 0, amx_idx = 0;
		cell amx_ret;

		for (Callback callback : info->callback_queue) {
			error = amx_FindPublic(amx, callback.function.c_str(), &amx_idx);

			if (error != AMX_ERR_NONE) {
				std::cout << "Failed to locate public function \"" << callback.function.c_str() << "\" in AMX, error: " << error << "\n";
				continue;
			}

			amx_Push(amx, callback.id);
			amx_Push(amx, callback.path);
			amx_Exec(amx, &amx_ret, amx_idx);
		}

		info->callback_queue.clear();
	}
}

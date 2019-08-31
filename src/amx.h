#pragma once

#include <vector>
#include <mutex>

#include "common.h"


class Amx
{
public:
	Amx(AMX* amx);
	void queueCallback(Callback* callback);
	void processCallbacks();
	AMX* getInternalAmx() const;

private:
	std::vector<Callback*> callback_queue_;
	std::mutex callback_queue_lock_;
	AMX* internal_amx_;
};

#include "amx.h"
#include "callback.h"


Amx::Amx(AMX* amx)
{
	internal_amx_ = amx;
}


void Amx::queueCallback(Callback* callback)
{
	callback_queue_lock_.lock();
	callback_queue_.push_back(callback);
	callback_queue_lock_.unlock();
}


void Amx::processCallbacks()
{
	if (callback_queue_lock_.try_lock())
	{
		for (auto callback : callback_queue_)
		{
			callback->call();
		}

		callback_queue_.clear();
		callback_queue_lock_.unlock();
	}
}


AMX* Amx::getInternalAmx() const
{
	return internal_amx_;
}

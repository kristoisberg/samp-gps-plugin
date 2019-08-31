#include <iostream>

#include "callback.h"
#include "amx.h"


Callback::Callback(Amx* amx, const std::string& function, const char* format, cell* params, const unsigned int param_offset)
{
	amx_ = amx;
	function_ = function;
	result_ = 0;

	if (format != nullptr)
	{
		const auto internal_amx = amx->getInternalAmx();
		cell* address = nullptr;
		auto param_index = param_offset + 1;

		do
		{
			switch (*format)
			{
			case 's':
			{
				char* string = nullptr;
				amx_StrParam(internal_amx, params[param_index++], string);
				arguments_.push_front(string == nullptr ? std::string() : std::string(string));
				break;
			}

			default:
			{
				amx_GetAddr(internal_amx, params[param_index++], &address);
				arguments_.push_front(*address);
				break;
			}
			}
		} while (*(++format));
	}
}


void Callback::setResult(const int value)
{
	result_ = value;
}


Amx* Callback::getAmx() const
{
	return amx_;
}


bool Callback::call()
{
	const auto internal_amx = amx_->getInternalAmx();
	auto amx_idx = 0;
	cell amx_ret;

	const auto error = amx_FindPublic(internal_amx, function_.c_str(), &amx_idx);

	if (error != AMX_ERR_NONE)
	{
		std::cout << "Failed to locate public function \"" << function_ << "\" in AMX, error: " << error << "\n";
		return false;
	}

	auto amx_address = -1;

	for (const auto& argument : arguments_)
	{
		if (argument.index() == 1)
		{
			cell tmp_address;

			amx_PushString(internal_amx, &tmp_address, nullptr, std::get<std::string>(argument).c_str(), 0, 0);

			if (amx_address == -1)
			{
				amx_address = tmp_address;
			}
		}
		else
		{
			amx_Push(internal_amx, std::get<int>(argument));
		}
	}

	amx_Push(internal_amx, result_);
	amx_Exec(internal_amx, &amx_ret, amx_idx);

	if (amx_address != -1)
	{
		amx_Release(internal_amx, amx_address);
	}

	return true;
}
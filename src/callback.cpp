#include <iostream>

#include "callback.h"
#include "amx.h"


Callback::Callback(AMX* amx, std::string function, const char* format, cell* params, unsigned int param_offset) {
    this->amx = amx;
    this->function = function;

    if (format != NULL) {
        cell* addr = NULL;
        unsigned int param_index = param_offset + 1;

        do {
            switch (*format) {
                case 'd':
                case 'i':
                case 'f': {
                    amx_GetAddr(amx, params[param_index++], &addr);
                    this->arguments.push_front(*addr);
                    break;
                }

                case 's': {
                    char* string = NULL;
                    amx_StrParam(amx, params[param_index++], string);
                    this->arguments.push_front(string == NULL ? std::string() : std::string(string));
                    break;
                }
            }
        } while (*(++format));
    }
}


void Callback::setResult(int value) { 
    this->result = value;
}


AMX* Callback::getAMX() { 
    return this->amx;
}


bool Callback::call() {
    int amx_idx = 0;
	cell amx_ret;

    int error = amx_FindPublic(amx, this->function.c_str(), &amx_idx);

    if (error != AMX_ERR_NONE) {
        std::cout << "Failed to locate public function \"" << this->function.c_str() << "\" in AMX, error: " << error << "\n";
        return false;
    }

    cell amx_addr = -1;

    for (std::variant<int, std::string> argument : arguments) {
        if (argument.index() == 1) {
            cell tmp_addr;

            amx_PushString(amx, &tmp_addr, NULL, std::get<std::string>(argument).c_str(), 0, 0);

            if (amx_addr == -1) {
                amx_addr = tmp_addr;
            }
        } else {
            amx_Push(amx, std::get<int>(argument));
        }
    }

    amx_Push(amx, this->result);
    amx_Exec(amx, &amx_ret, amx_idx);

    if (amx_addr != -1) {
        amx_Release(amx, amx_addr);
    }

    return true;
}
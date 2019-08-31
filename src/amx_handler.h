#pragma once


namespace AmxHandler
{
	void LoadAmx(AMX* amx);
	void UnloadAmx(AMX* amx);
	Amx* GetAmx(AMX* amx);
	void ProcessTick();
};
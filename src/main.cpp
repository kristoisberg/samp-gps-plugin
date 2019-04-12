#include "common.h"
#include "natives.h"
#include "amx.h"
#include "file.h"


logprintf_t logprintf;


PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}


PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) 
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	int line_count = 0, node_count = 0, connection_count = 0;
	
	if (!File::LoadNodes("scriptfiles/GPS.dat", line_count, node_count, connection_count)) 
	{
		logprintf("[GPS plugin]: Failed to open \"GPS.dat\"!");
	} 
	else 
	{
		logprintf("[GPS plugin]: Read %i lines, loaded %i nodes and %i connections.", line_count, node_count, connection_count);
	}

	return true;
}


PLUGIN_EXPORT void PLUGIN_CALL Unload() 
{
	logprintf("[GPS plugin]: Unloaded.");
}


AMX_NATIVE_INFO PluginNatives[] = 
{
	{"IsValidMapNode", 				Natives::IsValidMapNode},
	{"GetMapNodePos",				Natives::GetMapNodePos},
	{"GetDistanceBetweenMapNodes",	Natives::GetDistanceBetweenMapNodes},
	{"GetAngleBetweenMapNodes",		Natives::GetAngleBetweenMapNodes},
	{"GetMapNodeDistanceFromPoint",	Natives::GetMapNodeDistanceFromPoint},
	{"GetMapNodeAngleFromPoint",	Natives::GetMapNodeAngleFromPoint},
	{"GetClosestMapNodeToPoint",	Natives::GetClosestMapNodeToPoint},
	{"GetMapNodeConnectionCount",	Natives::GetMapNodeConnectionCount},
	{"GetHighestMapNodeID",			Natives::GetHighestMapNodeID},
	{"GetRandomMapNode",			Natives::GetRandomMapNode},

	{"FindPath",					Natives::FindPath},
	{"FindPathThreaded",			Natives::FindPathThreaded},

	{"IsValidPath", 				Natives::IsValidPath},
	{"GetPathSize",					Natives::GetPathSize},
	{"GetPathLength",				Natives::GetPathLength},
	{"GetPathNode",					Natives::GetPathNode},
	{"GetPathNodeIndex",			Natives::GetPathNodeIndex},
	{"DestroyPath",					Natives::DestroyPath},

	{0, 0}
};


PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx) 
{
	amx::Load(amx);

	return amx_Register(amx, PluginNatives, -1);
}


PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX* amx) 
{
    amx::Unload(amx);

    return 1;
}


PLUGIN_EXPORT void PLUGIN_CALL ProcessTick() 
{
    for (const std::pair<AMX*, AMXInfo> i : amx_list) 
	{
        amx::ProcessTick(i.first);
    }
}
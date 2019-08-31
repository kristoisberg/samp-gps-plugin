#include "common.h"
#include "natives.h"
#include "file.h"
#include "amx_handler.h"
#include "config_reader.h"


logprintf_t logprintf;


PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}


PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)(ppData[PLUGIN_DATA_LOGPRINTF]);

	auto config = ConfigReader("server.cfg");

	if (config.getValue<int>("gps_load_nodes", 1) != 0)
	{
		auto line_count = 0, node_count = 0, connection_count = 0;

		if (!File::LoadNodes("GPS.dat", line_count, node_count, connection_count))
		{
			logprintf("[GPS plugin]: Failed to open \"GPS.dat\"!");
		}
		else
		{
			logprintf("[GPS plugin]: Read %i lines, loaded %i nodes and %i connections.", line_count, node_count, connection_count);
		}
	}

	return true;
}


PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	logprintf("[GPS plugin]: Unloaded.");
}


AMX_NATIVE_INFO PluginNatives[] =
{
	{"CreateMapNode",					Natives::CreateMapNode},
	{"DestroyMapNode",					Natives::DestroyMapNode},
	{"IsValidMapNode", 					Natives::IsValidMapNode},
	{"GetMapNodePos",					Natives::GetMapNodePos},

	{"CreateConnection",				Natives::CreateConnection},
	{"DestroyConnection",				Natives::DestroyConnection},
	{"GetConnectionSource", 			Natives::GetConnectionSource},
	{"GetConnectionTarget",				Natives::GetConnectionTarget},

	{"GetMapNodeConnectionCount",		Natives::GetMapNodeConnectionCount},
	{"GetMapNodeConnection",			Natives::GetMapNodeConnection},
	{"GetConnectionBetweenMapNodes",	Natives::GetConnectionBetweenMapNodes},

	{"GetDistanceBetweenMapNodes",		Natives::GetDistanceBetweenMapNodes},
	{"GetAngleBetweenMapNodes",			Natives::GetAngleBetweenMapNodes},
	{"GetMapNodeDistanceFromPoint",		Natives::GetMapNodeDistanceFromPoint},
	{"GetMapNodeAngleFromPoint",		Natives::GetMapNodeAngleFromPoint},
	{"GetClosestMapNodeToPoint",		Natives::GetClosestMapNodeToPoint},
	
	{"GetHighestMapNodeID",				Natives::GetHighestMapNodeID},
	{"GetRandomMapNode",				Natives::GetRandomMapNode},

	{"FindPath",						Natives::FindPath},
	{"FindPathThreaded",				Natives::FindPathThreaded},

	{"SaveMapNodesToFile",				Natives::SaveMapNodesToFile},

	{"IsValidPath", 					Natives::IsValidPath},
	{"GetPathSize",						Natives::GetPathSize},
	{"GetPathLength",					Natives::GetPathLength},
	{"GetPathNode",						Natives::GetPathNode},
	{"GetPathNodeIndex",				Natives::GetPathNodeIndex},
	{"DestroyPath",						Natives::DestroyPath},

	{0, 0}
};


PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	AmxHandler::LoadAmx(amx);

	return amx_Register(amx, PluginNatives, -1);
}


PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX* amx)
{
	AmxHandler::UnloadAmx(amx);

	return 1;
}


PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	AmxHandler::ProcessTick();
}
#include <fstream>
#include <string>
#include <iostream>

#include "../lib/sdk/amx/amx.h"
#include "../lib/sdk/plugincommon.h"

#include "common.h"
#include "pathfinder.h"
#include "natives.h"


logprintf_t logprintf;


PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}


PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	std::ifstream file("scriptfiles/GPS.dat");

	if (!file.good()) {
		logprintf("[GPS plugin]: Failed to open \"GPS.dat\"!");
	} else {
		std::string buffer;
		int type = 0, id = 0, id2 = 0, direction = 0, line_count = 0, node_count = 0, connection_count = 0;
		float x = 0.0f, y = 0.0f, z = 0.0f;

		while (!file.eof()) {
			std::getline(file, buffer);
			sscanf(buffer.c_str(), "%i", &type);

			switch (type) {
				case 0: {
					sscanf(buffer.c_str(), "%*i %f %f %f %*i %i", &x, &y, &z, &id);

					if (Pathfinder::AddNode(id, x, y, z)) {
						node_count++;
					}

					break;
				}

				case 1: {
					sscanf(buffer.c_str(), "%*i %i %i %i", &id, &id2, &direction);

					if (direction != 2 && Pathfinder::AddConnection(id, id2)) {
						connection_count++;
					}

					break;
				}
			}

			line_count++;
		}

		file.close();

		logprintf("[GPS plugin]: Read %i lines, loaded %i nodes and %i connections.", line_count, node_count, connection_count);
	}
	
	return true;
}


PLUGIN_EXPORT void PLUGIN_CALL Unload() {
	logprintf("[GPS plugin]: Unloaded.");
}


AMX_NATIVE_INFO PluginNatives[] = {
	{"IsValidNode", 			Natives::IsValidNode},
	{"GetNodePos",				Natives::GetNodePos},
	{"FindPathFromNodeToNode",	Natives::FindPathFromNodeToNode},
	{"IsValidPath", 			Natives::IsValidPath},
	{"GetPathSize",				Natives::GetPathSize},
	{"GetPathLength",			Natives::GetPathLength},
	{"GetPathNode",				Natives::GetPathNode},
	{0, 0}
};


PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx) {
	return amx_Register(amx, PluginNatives, -1);
}

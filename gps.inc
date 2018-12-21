#if defined _inc_gps
    #undef _inc_gps
#endif

#if defined _gps_included
    #endinput
#endif
#define _gps_included


#define INVALID_NODE_ID MapNode:-1
#define INVALID_PATH_ID Path:-1


native bool:IsValidNode(MapNode:nodeid);
native GetNodePos(MapNode:nodeid, &Float:x, &Float:y, &Float:z);
native Path:FindPathFromNodeToNode(MapNode:start, MapNode:target);
native bool:IsValidPath(Path:pathid);
native GetPathSize(Path:pathid);
native GetPathLength(Path:pathid);
native MapNode:GetPathNode(Path:pathid, index);
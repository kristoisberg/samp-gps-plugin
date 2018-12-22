#if defined _inc_gps
    #undef _inc_gps
#endif

#if defined _gps_included
    #endinput
#endif
#define _gps_included


#define INVALID_MAP_NODE_ID MapNode:-1
#define INVALID_PATH_ID     Path:-1


/*enum _:GPS_ERROR {
    GPS_ERROR_NONE              =  0,
    GPS_ERROR_INVALID_PARAMS    = -1,
    GPS_ERROR_INVALID_PATH      = -2,
    GPS_ERROR_INVALID_NODE      = -3
}


native bool:IsValidMapNode(MapNode:nodeid);
native GetMapNodePos(MapNode:nodeid, &Float:x, &Float:y, &Float:z);
native GetDistanceBetweenMapNodes(MapNode:first, MapNode:second, &Float:distance);
native GetMapNodeDistanceFromPoint(MapNode:nodeid, Float:x, Float:y, Float:z, &Float:distance);
native GetClosestMapNodeToPoint(Float:x, Float:y, Float:z, &MapNode:nodeid, MapNode:ignorednode = INVALID_MAP_NODE_ID);

native FindPath(MapNode:start, MapNode:target, &Path:pathid);
native FindPathThreaded(MapNode:start, MapNode:target, const callback[], id = 0);

native bool:IsValidPath(Path:pathid);
native GetPathSize(Path:pathid, &size);
native GetPathLength(Path:pathid, &Float:length);
native GetPathNode(Path:pathid, index, &MapNode:nodeid);
native DestroyPath(Path:pathid);*/


native bool:IsValidNode(MapNode:nodeid);
native GetNodePos(MapNode:nodeid, &Float:x, &Float:y, &Float:z);
native Path:FindPathFromNodeToNode(MapNode:start, MapNode:target);
native FindPathFromNodeToNodeThreaded(MapNode:start, MapNode:target, const callback[], id = 0);
native bool:IsValidPath(Path:pathid);
native GetPathSize(Path:pathid);
native Float:GetPathLength(Path:pathid);
native MapNode:GetPathNode(Path:pathid, index);
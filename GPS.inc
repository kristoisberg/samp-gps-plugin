#if defined _inc_gps
    #undef _inc_gps
#endif

#if defined _gps_included
    #endinput
#endif
#define _gps_included


const MapNode:INVALID_MAP_NODE_ID = MapNode:-1;
const Path:INVALID_PATH_ID = Path:-1;
const Connection:INVALID_CONNECTION_ID = Connection:-1;

enum _:GPS_ERROR 
{
    GPS_ERROR_NONE                  =  0,
    GPS_ERROR_INVALID_PARAMS        = -1,
    GPS_ERROR_INVALID_PATH          = -2,
    GPS_ERROR_INVALID_NODE          = -3,
    GPS_ERROR_INVALID_CONNECTION    = -4,
    GPS_ERROR_INTERNAL              = -5
}


native CreateMapNode(Float:x, Float:y, Float:z, &MapNode:nodeid);
native DestroyMapNode(MapNode:nodeid);
native bool:IsValidMapNode(MapNode:nodeid);
native GetMapNodePos(MapNode:nodeid, &Float:x, &Float:y, &Float:z);

native CreateConnection(MapNode:source, MapNode:target, &Connection:connectionid);
native DestroyConnection(Connection:connectionid);
native GetConnectionSource(Connection:connectionid, &MapNode:nodeid);
native GetConnectionTarget(Connection:connectionid, &MapNode:nodeid);

native GetMapNodeConnectionCount(MapNode:nodeid, &count);
native GetMapNodeConnection(MapNode:nodeid, index, &Connection:connectionid);
native GetConnectionBetweenMapNodes(MapNode:source, MapNode:target, &Connection:connectionid);

native GetDistanceBetweenMapNodes(MapNode:first, MapNode:second, &Float:distance);
native GetAngleBetweenMapNodes(MapNode:first, MapNode:second, &Float:angle);
native GetMapNodeDistanceFromPoint(MapNode:nodeid, Float:x, Float:y, Float:z, &Float:distance);
native GetMapNodeAngleFromPoint(MapNode:nodeid, Float:x, Float:y, &Float:angle);
native GetClosestMapNodeToPoint(Float:x, Float:y, Float:z, &MapNode:nodeid, MapNode:ignorednode = INVALID_MAP_NODE_ID);

native GetHighestMapNodeID();
native GetRandomMapNode(&MapNode:nodeid);

native SaveMapNodesToFile(const filename[]);

native FindPath(MapNode:source, MapNode:target, &Path:pathid);
native FindPathThreaded(MapNode:source, MapNode:target, const callback[], const format[] = "", {Float, _}:...);

native bool:IsValidPath(Path:pathid);
native GetPathSize(Path:pathid, &size);
native GetPathLength(Path:pathid, &Float:length);
native GetPathNode(Path:pathid, index, &MapNode:nodeid);
native GetPathNodeIndex(Path:pathid, MapNode:nodeid, &index);
native DestroyPath(Path:pathid);


#if defined _PawnPlus_included
    stock Task:FindPathAsync(MapNode:source, MapNode:target) 
    {
        new Task:task = task_new();

        if (FindPathThreaded(source, target, "FindPathAsyncResponse", "i", _:task) != GPS_ERROR_NONE) 
        {
            task_set_error_ticks(task, amx_err_exit, 0);
        }

        return task;
    }

    forward public FindPathAsyncResponse(Path:path, Task:task);
    public FindPathAsyncResponse(Path:path, Task:task) 
    {
        task_set_result(task, _:path);
    }
#endif
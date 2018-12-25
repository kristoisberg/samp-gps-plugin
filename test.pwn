#include <a_samp>
#include <PawnPlus>

#include "GPS.inc"


#define THREADING_TEST_COUNT 100

new ThreadingTestCount;


main() {
    new MapNode:nodeid, error = GetClosestMapNodeToPoint(-2427.625000, -2474.750000, 35.750000, nodeid);
    
    if (error) {
        printf("GetClosestMapNodeToPoint: %i", error);
    } else if (nodeid != MapNode:0) {
        new Float:dist1, Float:dist2;
        GetMapNodeDistanceFromPoint(nodeid, -2427.625000, -2474.750000, 35.750000, dist1);
        GetMapNodeDistanceFromPoint(MapNode:0, -2427.625000, -2474.750000, 35.750000, dist2);
        printf("nodeid: %i %f %f", _:nodeid, dist1, dist2);
    } else {
        new Float:distance;
        error = GetMapNodeDistanceFromPoint(nodeid, -2427.625000, -2474.750000, 35.750000, distance);

        if (error) {
            printf("GetMapNodeDistanceFromPoint: %i", error);
        } else if (distance > 0.001) {
            printf("distance: %f", distance);
        }
    }

    new connection_count, Float:angle, Float:angle2, MapNode:random_node;
    GetRandomMapNode(random_node);
    GetMapNodeConnectionCount(nodeid, connection_count);
    GetAngleBetweenMapNodes(nodeid, random_node, angle);
    GetMapNodeAngleFromPoint(nodeid, 0.0, 0.0, angle2);

    printf("%i %i %i %i %f %f", GetHighestMapNodeID(), connection_count, _:random_node, IsValidMapNode(random_node), angle, angle2);

    Test(MapNode:1,  MapNode:2);
    Test(MapNode:82, MapNode:90);
    Test(MapNode:1,  MapNode:27751);

    for (new i; i < THREADING_TEST_COUNT; i++) {
        FindPathThreaded(MapNode:1, MapNode:27751, "ThreadingTestResponse");
    }

    FindPathThreaded(MapNode:82, MapNode:90, "TestResponse2", "ifis", 1337, 3.14, true, "i'm a string");
}

forward public ThreadingTestResponse(Path:path);
public ThreadingTestResponse(Path:path) {
    ThreadingTestCount++;
    printf("%i/%i", ThreadingTestCount, THREADING_TEST_COUNT);
}


Test(MapNode:start, MapNode:target) {
    new Float:x, Float:y, Float:z;

    GetMapNodePos(start, x, y, z);
    printf("Start: %f %f %f", x, y, z);

    GetMapNodePos(target, x, y, z);
    printf("Target: %f %f %f", x, y, z);

    new tick = GetTickCount(), Path:path;
    FindPath(start, target, path);

    printf("NON-THREADED: %ims", GetTickCount() - tick);
    Test2(path);

    FindPathThreaded(start, target, "TestResponse", "i", GetTickCount());

    tick = GetTickCount();
    path = Path:task_await(FindPathAsync(start, target));

    printf("ASYNC: %ims", GetTickCount() - tick);
    Test2(path);
}


Test2(Path:path) {
    new Float:x, Float:y, Float:z, size, Float:length, MapNode:node;
    GetPathSize(path, size);
    GetPathLength(path, length);

    printf("Valid: %s | Distance: %fm | Amount of nodes: %i", IsValidPath(path) ? ("Yes") : ("No"), length, size);

    for (new i; i < size; i++) {
        GetPathNode(path, i, node);
        GetMapNodePos(node, x, y, z);
        printf("%i: %f %f %f", i + 1, x, y, z);
    }

    DestroyPath(path);
    printf("Destroyed | Valid: %s", IsValidPath(path) ? ("Yes") : ("No"));
}


forward public TestResponse(Path:path, tick);
public TestResponse(Path:path, tick) {
    printf("THREADED: %ims", GetTickCount() - tick);
    Test2(path);
}


forward public TestResponse2(Path:path, integer, Float:float, bool:boolean, const string[]);
public TestResponse2(Path:path, integer, Float:float, bool:boolean, const string[]) {
    printf("%i %i %f %i %s", _:path, integer, float, boolean, string);
}
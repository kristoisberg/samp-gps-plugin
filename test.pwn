#include <a_samp>

#include "gps.inc"


main() {
    Test(MapNode:1,  MapNode:2);
    Test(MapNode:82, MapNode:90);
    Test(MapNode:1,  MapNode:27751);

    /*new MapNode:start = MapNode:1, MapNode:target = MapNode:27751, Float:x, Float:y, Float:z;

    GetNodePos(start, x, y, z);
    printf("Start: %f %f %f", x, y, z);

    GetNodePos(target, x, y, z);
    printf("Target: %f %f %f", x, y, z);

    new tick = GetTickCount(), Path:path = FindPathFromNodeToNode(start, target);

    printf("non-threaded: %ims", GetTickCount() - tick);

    new size = GetPathSize(path);

    printf("NON-THREADED:");
    printf("Valid: %s | Distance: %fm | Amount of nodes: %i", IsValidPath(path) ? ("Yes") : ("No"), GetPathLength(path), size);

    for (new i; i < size; i++) {
        GetNodePos(GetPathNode(path, i), x, y, z);
        printf("%i: %f %f %f", i + 1, x, y, z);
    }

    FindPathFromNodeToNodeThreaded(start, target, "OnGPSResponse", GetTickCount());*/
}


Test(MapNode:start, MapNode:target) {
    new Float:x, Float:y, Float:z;

    GetNodePos(start, x, y, z);
    printf("Start: %f %f %f", x, y, z);

    GetNodePos(target, x, y, z);
    printf("Target: %f %f %f", x, y, z);

    new start_tick = GetTickCount(), Path:path = FindPathFromNodeToNode(start, target), end_tick = GetTickCount(), size = GetPathSize(path);

    printf("Time: %ims | Valid: %s | Distance: %fm | Amount of nodes: %i", end_tick - start_tick, IsValidPath(path) ? ("Yes") : ("No"), GetPathLength(path), size);

    for (new i; i < size; i++) {
        GetNodePos(GetPathNode(path, i), x, y, z);
        printf("%i: %f %f %f", i + 1, x, y, z);
    }
}


/*forward public OnGPSResponse(Path:path, tick);
public OnGPSResponse(Path:path, tick) {
    printf("threaded: %ims", GetTickCount() - tick);

    new size = GetPathSize(path), Float:x, Float:y, Float:z;

    printf("THREADED:");
    printf("Valid: %s | Distance: %fm | Amount of nodes: %i", IsValidPath(path) ? ("Yes") : ("No"), GetPathLength(path), size);

    for (new i; i < size; i++) {
        GetNodePos(GetPathNode(path, i), x, y, z);
        printf("%i: %f %f %f", i + 1, x, y, z);
    }
}*/
#include <a_samp>

#include "gps.inc"


main() {
    new 
        MapNode:start = MapNode:82, 
        MapNode:target = MapNode:90,
        Float:x,
        Float:y,
        Float:z,
        Path:path = FindPathFromNodeToNode(start, target),
        size = GetPathSize(path)
    ;

    GetNodePos(start, x, y, z);
    printf("Start: %f %f %f", x, y, z);

    GetNodePos(target, x, y, z);
    printf("Target: %f %f %f", x, y, z);

    printf("Distance: %fm | Amount of nodes: %i", GetPathLength(path), size);

    for (new i; i < size; i++) {
        GetNodePos(GetPathNode(path, i), x, y, z);
        printf("%i: %f %f %f", i + 1, x, y, z);
    }
}
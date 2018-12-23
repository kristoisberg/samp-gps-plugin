#include <a_samp>
#include <PawnPlus>

#include "gps.inc"


#define THREADING_TEST_COUNT 100

new ThreadingTestCount;


main() {
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
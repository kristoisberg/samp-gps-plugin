# SA-MP GPS Plugin

[![sampctl](https://shields.southcla.ws/badge/sampctl-samp--gps--plugin-2f2f2f.svg?style=for-the-badge)](https://github.com/kristoisberg/samp-gps-plugin)

This plugin offers a way of accessing the data of San Andreas map nodes and finding paths between them. It is intended to be a modern and straightforward replacement for RouteConnector. The plugin uses a simple implementation of the A* algorithm for pathfinding. Finding a path from the top-leftmost node on the map to the bottom-rightmost node that consists of 684 nodes takes just under one second and most shorter paths only take a few milliseconds. It is worth noting that this is my first successful project in C++ and the last time I tried writing something in C++ was over half a year ago so there is a lot of room for improvement and I am planning to continue improving the code of this plugin as I progress.


### Advantages over RouteConnector

* **Safer API** - Unlike RouteConnector, this plugins does not give you an array of nodes as the result of pathfinding. Instead of that, it gives you the ID of the found path that can be used later on. Each function (except `IsValidMapNode` and `IsValidPath`) returns an error code and the real result of them is passed by reference.
* **Compatibility** - RouteConnector has a compatibility issue with some part of YSI that makes it call a wrong public function instead of the actual `GPS_WhenRouteIsCalculated` callback. This plugin lets you call a custom callback and pass arguments to it. In addition to that, RouteConnector uses Intel Threading Building Blocks for threading that has caused numerous compatibility (and PEBCAK) issues on Linux servers. This plugin uses `std::thread` for threading and does not have any dependencies. This plugin is also compatible with PawnPlus and supports asynchronous pathfinding out of box.
* **Active development** - RouteConnector has not been updated for over three years. As I said previously, I am planning to continue active development of this plugin for a long period of time.


### Disadvantages over RouteConnector

* **Performance** - I highly doubt that the plugin performs better than RouteConnector, but since most pathfinding tasks only take a fraction of a second, there should be absolutely no performance issues as long as threaded/asynchronous pathfinding functions are used.
* **Functionality** - At the current time, this plugin only replaces RouteConnector in the areas of accessing map node data and pathfinding. There is no way of managing nodes or their connections in runtime or modifying the `GPS.dat` file.


## Installation

Simply install to your project:

```bash
sampctl package install kristoisberg/samp-gps-plugin
```

Include in your code and begin using the library:

```pawn
#include <GPS>
```


## API


### Functions

`bool:IsValidMapNode(MapNode:nodeid)`

* Returns if the map node with the specified ID is valid.

`GetMapNodePos(MapNode:nodeid, &Float:x, &Float:y, &Float:z)`

* If the specified map node is valid, returns `GPS_ERROR_NONE` and passes the position of it to `x`, `y` and `z`, otherwise returns `GPS_ERROR_INVALID_NODE`.

`GetDistanceBetweenMapNodes(MapNode:first, MapNode:second, &Float:distance)`

* If both of the specified map nodes are valid, returns `GPS_ERROR_NONE` and passes the distance between them to `distance`, otherwise returns `GPS_ERROR_INVALID_NODE`.

`GetMapNodeDistanceFromPoint(MapNode:nodeid, Float:x, Float:y, Float:z, &Float:distance)`

* If the specified map node is valid, returns `GPS_ERROR_NONE` and passes the distance of the map node from the specified position to `distance`, otherwise returns `GPS_ERROR_INVALID_NODE`.

`GetClosestMapNodeToPoint(Float:x, Float:y, Float:z, &MapNode:nodeid, MapNode:ignorednode = INVALID_MAP_NODE_ID)`

* Passes the closest map node to the specified position to `nodeid`. If `ignorednode` is specified and it is the closest node to the position, it is ignored and the next closest node is returned instead.

`FindPath(MapNode:start, MapNode:target, &Path:pathid)`

* If both of the specified map nodes are valid, returns `GPS_ERROR_NONE` and tries to find a path from `start` to `target` and pass its ID to `pathid`, otherwise returns `GPS_ERROR_INVALID_NODE`. If pathfinding fails, returns `GPS_ERROR_INVALID_PATH`.

`FindPathThreaded(MapNode:start, MapNode:target, const callback[], const format[] = "", {Float, _}:...)`

* If both of the specified map nodes are valid, returns `GPS_ERROR_NONE` and tries to find a path from `start` to `target`. After pathfinding is finished, calls the specified callback and passes the path ID (could be `INVALID_PATH_ID` if pathfinding fails) and the specified arguments to it.

`Task:FindPathAsync(MapNode:start, MapNode:target)`

* Pauses the current function and continues it after it is finished. Throws an AMX error if pathfinding fails for any reason. Only available if PawnPlus is included before GPS. Usage explained below.

`bool:IsValidPath(Path:pathid)`

* Returns if the path with the specified ID is valid.

`GetPathSize(Path:pathid, &size)`

* If the specified path is valid, returns `GPS_ERROR_NONE` and passes the amount of nodes in it to `size`, otherwise returns `GPS_ERROR_INVALID_PATH`.

`GetPathNode(Path:pathid, index, &MapNode:nodeid)`

* If the specified path is valid and the index contains a node, returns `GPS_ERROR_NONE` and passes the ID of the node at that index to `nodeid`, otherwise returns `GPS_ERROR_INVALID_PATH` or `GPS_ERROR_INVALID_NODE` depending on the error.

`GetPathLength(Path:pathid, &Float:length)`

* If the specified path is valid, returns `GPS_ERROR_NONE` and passes the length of the path in metres to `length`, otherwise returns `GPS_ERROR_INVALID_PATH`.

`DestroyPath(Path:pathid)`

* If the specified path is valid, returns `GPS_ERROR_NONE` and destroys the path, otherwise returns `GPS_ERROR_INVALID_PATH`.


### Error codes

* `GPS_ERROR_NONE` - The function was executed successfully.
* `GPS_ERROR_INVALID_PARAMS` - An invalid amount of arguments was passed to the function. Should never happen without the PAWN compiler noticing it unless the versions of the plugin and include are different.
* `GPS_ERROR_INVALID_PATH` - An invalid path ID as passed to the function or threaded pathfinding was not successful.
* `GPS_ERROR_INVALID_NODE` - An invalid map node ID/index was passed to the function or `GetClosestMapNodeToPoint` failed because `GPS.dat` failed to load.
* `GPS_ERROR_INTERNAL` - An internal error happened - threaded pathfinding failed because dispatching a thread failed.


## Examples


### Threaded pathfinding

Finding a path from the position of the player to the LSPD building.

```pawn
CMD:pathtols(playerid) {
    new Float:x, Float:y, Float:z, MapNode:start;
    GetPlayerPos(playerid, x, y, z);

    if (GetClosestMapNodeToPoint(x, y, z, start) != GPS_ERROR_NODE) {
        return SendClientMessage(playerid, COLOR_RED, "Finding a node near you failed, GPS.dat was not loaded.");
    }

    new MapNode:target;

    if (!GetClosestMapNodeToPoint(1258.7352, -2036.7100, 59.4561, target)) { // this is also valid since the value of GPS_ERROR_NODE is 0.
        return SendClientMessage(playerid, COLOR_RED, "Finding a node near LSPD failed, GPS.dat was not loaded.");
    }

    if (!FindPathThreaded(start, target, "OnPathToLSFound", "ii", playerid, GetTickCount())) {
        return SendClientMessage(playerid, COLOR_RED, "Pathfinding failed for some reason, you should store this error code and print it out since there are multiple ways it could fail.");
    }

    SendClientMessage(playerid, COLOR_WHITE, "Finding the path...");
    return 1;
}


forward public OnPathToLSFound(Path:pathid, playerid, start_time);
public OnPathToLSFound(Path:pathid, playerid, start_time) {
    if (!IsValidPath(pathid)) {
        return SendClientMessage(playerid, COLOR_RED, "Pathfinding failed!");
    }

    new string[128], size, length;
    GetPathSize(size);
    GetPathLength(length);

    format(string, sizeof(string), "Found a path in %ims. Amount of nodes: %i, length: %fm.", GetTickCount() - start_time, size, length);

    new MapNode:nodeid, Float:x, Float:y, Float:z;

    for (new index; index < size; index++) {
        GetPathNode(pathid, index, nodeid);
        GetMapNodePos(nodeid, x, y, z);
        CreateDynamicPickup(1318, 1, x, y, z);
    }

    DestroyPath(pathid);
    return 1;
}
```


### Asynchronous pathfinding

What if you could continue the process within the command while still taking advantage of the benefits of threaded pathfinding? You can, using the magic of PawnPlus tasks.

```pawn
CMD:pathtols(playerid) {
    new Float:x, Float:y, Float:z, MapNode:start;
    GetPlayerPos(playerid, x, y, z);

    if (!GetClosestMapNodeToPoint(x, y, z, start)) {
        return SendClientMessage(playerid, COLOR_RED, "Finding a node near you failed, GPS.dat was not loaded.");
    }

    new MapNode:target;

    if (!GetClosestMapNodeToPoint(1258.7352, -2036.7100, 59.4561, target)) { 
        return SendClientMessage(playerid, COLOR_RED, "Finding a node near LSPD failed, GPS.dat was not loaded.");
    }

    SendClientMessage(playerid, COLOR_WHITE, "Finding the path...");

    new Path:pathid = task_await(FindPathAsync(start, target)); // no error handling here, an AMX error will be thrown instead if the pathfinding fails

    new string[128], size, length;
    GetPathSize(size);
    GetPathLength(length);

    format(string, sizeof(string), "Found a path in %ims. Amount of nodes: %i, length: %fm.", GetTickCount() - start_time, size, length);

    new MapNode:nodeid, Float:x, Float:y, Float:z, index;

    while (!GetPathNode(pathid, index, nodeid)) // also note the alternative method of iterating through path nodes here
        GetMapNodePos(nodeid, x, y, z);
        CreateDynamicPickup(1318, 1, x, y, z);

        index++;
    }

    DestroyPath(pathid);
    return 1;
}
```


## Testing

To test, simply run the package:

```bash
sampctl package run
```


## Credits

* kvann - Creator of the plugin.
* Gamer_Z - Creator of the original RouteConnector plugin which helped me understand the structure of GPS.dat and influenced this plugin a lot. Also the author of the original GPS.dat?
* NaS - Author of the fixed GPS.dat distributed with the plugin.
* Southclaws, IllidanS4, Yashas, J0sh, Johnson_boy/lassir - Helped me with the plugin in some way or another.
#include <map>
#include <thread>
#include <iostream>
#include <random>
#include <algorithm>

#include "natives.h"
#include "callback.h"
#include "node.h"
#include "path.h"


namespace Natives
{
	std::mt19937 mersenne{ std::random_device{}() };


	cell AMX_NATIVE_CALL IsValidMapNode(AMX* amx, cell* params)
	{
		CHECK_PARAMS(1);

		const auto id = static_cast<int>(params[1]);

		return Container::GetNodeByID(id) != nullptr;
	}


	cell AMX_NATIVE_CALL GetHighestMapNodeID(AMX* amx, cell* params)
	{
		return Container::GetHighestNodeID();
	}


	cell AMX_NATIVE_CALL GetRandomMapNode(AMX* amx, cell* params)
	{
		CHECK_PARAMS(1);

		auto nodes = Container::GetNodes();

		if (nodes.empty())
		{
			return GPS_ERROR_INVALID_NODE;
		}

		std::vector<std::pair<int, Node*>> result;
		std::sample(nodes.begin(), nodes.end(), std::back_inserter(result), 1, mersenne);

		cell* address = nullptr;
		amx_GetAddr(amx, params[1], &address);
		*address = result.at(0).second->getID();

		return GPS_ERROR_NONE;
	}


	cell AMX_NATIVE_CALL GetMapNodePos(AMX* amx, cell* params)
	{
		CHECK_PARAMS(4);

		const auto id = static_cast<int>(params[1]);
		const auto node = Container::GetNodeByID(id);

		if (node == nullptr)
		{
			return GPS_ERROR_INVALID_NODE;
		}

		auto x = node->getX(), y = node->getY(), z = node->getZ();

		cell* address = nullptr;

		amx_GetAddr(amx, params[2], &address);
		*address = amx_ftoc(x);

		amx_GetAddr(amx, params[3], &address);
		*address = amx_ftoc(y);

		amx_GetAddr(amx, params[4], &address);
		*address = amx_ftoc(z);

		return GPS_ERROR_NONE;
	}


	cell AMX_NATIVE_CALL GetMapNodeConnectionCount(AMX* amx, cell* params)
	{
		CHECK_PARAMS(2);

		const auto id = static_cast<int>(params[1]);
		const auto node = Container::GetNodeByID(id);

		if (node == nullptr)
		{
			return GPS_ERROR_INVALID_NODE;
		}

		cell* address = nullptr;
		amx_GetAddr(amx, params[2], &address);
		*address = node->getConnections().size();

		return GPS_ERROR_NONE;
	}


	cell AMX_NATIVE_CALL GetDistanceBetweenMapNodes(AMX* amx, cell* params)
	{
		CHECK_PARAMS(3);

		const auto first_id = static_cast<int>(params[1]);
		const auto first = Container::GetNodeByID(first_id);

		if (first == nullptr)
		{
			return GPS_ERROR_INVALID_NODE;
		}

		const auto second_id = static_cast<int>(params[2]);
		const auto second = Container::GetNodeByID(second_id);

		if (second == nullptr)
		{
			return GPS_ERROR_INVALID_NODE;
		}

		auto distance = first->getDistanceFromNode(second);

		cell* address = nullptr;
		amx_GetAddr(amx, params[3], &address);
		*address = amx_ftoc(distance);

		return GPS_ERROR_NONE;
	}


	cell AMX_NATIVE_CALL GetMapNodeDistanceFromPoint(AMX* amx, cell* params)
	{
		CHECK_PARAMS(5);

		const auto id = static_cast<int>(params[1]);
		const auto node = Container::GetNodeByID(id);

		if (node == nullptr)
		{
			return GPS_ERROR_INVALID_NODE;
		}

		const auto x = amx_ctof(params[2]), y = amx_ctof(params[3]), z = amx_ctof(params[4]);

		auto distance = node->getDistanceFromPoint(x, y, z);

		cell* address = nullptr;
		amx_GetAddr(amx, params[5], &address);
		*address = amx_ftoc(distance);

		return GPS_ERROR_NONE;
	}


	cell AMX_NATIVE_CALL GetAngleBetweenMapNodes(AMX* amx, cell* params)
	{
		CHECK_PARAMS(3);

		const auto first_id = static_cast<int>(params[1]);
		const auto first = Container::GetNodeByID(first_id);

		if (first == nullptr)
		{
			return GPS_ERROR_INVALID_NODE;
		}

		const auto second_id = static_cast<int>(params[2]);
		const auto second = Container::GetNodeByID(second_id);

		if (second == nullptr)
		{
			return GPS_ERROR_INVALID_NODE;
		}

		auto angle = first->getAngleFromNode(second);

		cell* address = nullptr;
		amx_GetAddr(amx, params[3], &address);
		*address = amx_ftoc(angle);

		return GPS_ERROR_NONE;
	}


	cell AMX_NATIVE_CALL GetMapNodeAngleFromPoint(AMX* amx, cell* params)
	{
		CHECK_PARAMS(4);

		const auto id = static_cast<int>(params[1]);
		const auto node = Container::GetNodeByID(id);

		if (node == nullptr)
		{
			return GPS_ERROR_INVALID_NODE;
		}

		const auto x = amx_ctof(params[2]), y = amx_ctof(params[3]);

		auto angle = node->getAngleFromPoint(x, y);

		cell* address = nullptr;
		amx_GetAddr(amx, params[4], &address);
		*address = amx_ftoc(angle);

		return GPS_ERROR_NONE;
	}


	cell AMX_NATIVE_CALL GetClosestMapNodeToPoint(AMX* amx, cell* params)
	{
		CHECK_PARAMS(5);

		const auto x = amx_ctof(params[1]), y = amx_ctof(params[2]), z = amx_ctof(params[3]);
		const auto ignored = static_cast<int>(params[5]);
		auto result = INVALID_NODE_ID, id = INVALID_NODE_ID;
		auto distance = std::numeric_limits<float>::infinity(), temp = 0.0f;

		for (const auto node : Container::GetNodes())
		{
			id = node.second->getID();

			if (ignored == id)
			{
				continue;
			}

			temp = node.second->getDistanceFromPoint(x, y, z);

			if (temp < distance)
			{
				result = id;
				distance = temp;
			}
		}

		cell* address = nullptr;
		amx_GetAddr(amx, params[4], &address);
		*address = result;

		return GPS_ERROR_NONE;
	}


	cell AMX_NATIVE_CALL FindPath(AMX* amx, cell* params)
	{
		CHECK_PARAMS(3);

		const auto start_id = static_cast<int>(params[1]);
		const auto start = Container::GetNodeByID(start_id);

		if (start == nullptr)
		{
			return GPS_ERROR_INVALID_NODE;
		}

		const auto target_id = static_cast<int>(params[2]);
		const auto target = Container::GetNodeByID(target_id);

		if (target == nullptr)
		{
			return GPS_ERROR_INVALID_NODE;
		}

		const auto result = Pathfinder::FindPath(start, target);

		if (result == -1)
		{
			return GPS_ERROR_INVALID_PATH;
		}

		cell* address = nullptr;
		amx_GetAddr(amx, params[3], &address);
		*address = result;

		return GPS_ERROR_NONE;
	}


	cell AMX_NATIVE_CALL FindPathThreaded(AMX* amx, cell* params)
	{
		if (params[0] < (3 * 4))
		{
			std::cout << "FindPathThreaded: Expecting at least 3 parameter(s), but found " << (params[0] / sizeof(cell)) << ".\n";
			return GPS_ERROR_INVALID_PARAMS;
		}

		const auto start_id = static_cast<int>(params[1]);
		auto start = Container::GetNodeByID(start_id);

		if (start == nullptr)
		{
			return GPS_ERROR_INVALID_NODE;
		}

		const auto target_id = static_cast<int>(params[2]);
		auto target = Container::GetNodeByID(target_id);

		if (target == nullptr)
		{
			return GPS_ERROR_INVALID_NODE;
		}

		const auto callback_name = amx_GetCppString(amx, params[3]);

		char* format = nullptr;
		amx_StrParam(amx, params[4], format);

		auto callback = new Callback(amx, callback_name, format, params, 4);

		try
		{
			std::thread t(Pathfinder::FindPathThreaded, start, target, callback);
			t.detach();
		}
		catch (std::exception &e)
		{
			std::cout << "Failed to dispatch thread: " << e.what() << "\n";
			return GPS_ERROR_INTERNAL;
		}

		return GPS_ERROR_NONE;
	}


	cell AMX_NATIVE_CALL IsValidPath(AMX* amx, cell* params)
	{
		CHECK_PARAMS(1);

		const auto id = static_cast<int>(params[1]);

		return Container::GetPathByID(id) != nullptr;
	}


	cell AMX_NATIVE_CALL GetPathSize(AMX* amx, cell* params)
	{
		CHECK_PARAMS(2);

		const auto id = static_cast<int>(params[1]);
		const auto path = Container::GetPathByID(id);

		if (path == nullptr)
		{
			return GPS_ERROR_INVALID_PATH;
		}

		cell* address = nullptr;
		amx_GetAddr(amx, params[2], &address);
		*address = path->getNodes().size();

		return GPS_ERROR_NONE;
	}


	cell AMX_NATIVE_CALL GetPathLength(AMX* amx, cell* params)
	{
		CHECK_PARAMS(2);

		const auto id = static_cast<int>(params[1]);
		const auto path = Container::GetPathByID(id);

		if (path == nullptr)
		{
			return GPS_ERROR_INVALID_PATH;
		}

		auto result = path->getLength();

		cell* address = nullptr;
		amx_GetAddr(amx, params[2], &address);
		*address = amx_ftoc(result);

		return GPS_ERROR_NONE;
	}


	cell AMX_NATIVE_CALL GetPathNode(AMX* amx, cell* params)
	{
		CHECK_PARAMS(3);

		const auto id = static_cast<int>(params[1]);
		const auto path = Container::GetPathByID(id);

		if (path == nullptr)
		{
			return GPS_ERROR_INVALID_PATH;
		}

		const auto index = static_cast<unsigned int>(params[2]);
		auto nodes = path->getNodes();

		if (index < 0 || index >= nodes.size())
		{
			return GPS_ERROR_INVALID_NODE;
		}

		cell* address = nullptr;
		amx_GetAddr(amx, params[3], &address);
		*address = nodes.at(index)->getID();

		return GPS_ERROR_NONE;
	}


	cell AMX_NATIVE_CALL GetPathNodeIndex(AMX* amx, cell* params)
	{
		CHECK_PARAMS(3);

		const auto id = static_cast<int>(params[1]);
		const auto path = Container::GetPathByID(id);

		if (path == nullptr)
		{
			return GPS_ERROR_INVALID_PATH;
		}

		const auto node_id = static_cast<int>(params[2]);
		const auto node = Container::GetNodeByID(node_id);

		if (node == nullptr)
		{
			return GPS_ERROR_INVALID_NODE;
		}

		auto nodes = path->getNodes();

		for (unsigned int i = 0; i < nodes.size(); i++)
		{
			if (nodes.at(i) == node)
			{
				cell* address = nullptr;
				amx_GetAddr(amx, params[3], &address);
				*address = i;

				return GPS_ERROR_NONE;
			}
		}

		return GPS_ERROR_INVALID_NODE;
	}


	cell AMX_NATIVE_CALL DestroyPath(AMX* amx, cell* params)
	{
		CHECK_PARAMS(1);

		const auto id = static_cast<int>(params[1]);
		const auto path = Container::GetPathByID(id);

		if (path == nullptr)
		{
			return GPS_ERROR_INVALID_PATH;
		}

		Container::RemovePath(path);
		return GPS_ERROR_NONE;
	}
}
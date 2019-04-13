#include <cmath>
#include <queue>
#include <exception>
#include <functional>

#include "pathfinder.h"
#include "connection.h"
#include "amx.h"


namespace Pathfinder
{
	float infinity = std::numeric_limits<float>::infinity();

	struct VisitedNode
	{
		Node* node;
		VisitedNode* previous;
		float distance = infinity;
		float target_distance = infinity;
		float total_distance = infinity;
	};


	bool NodeComparator(const VisitedNode* lhs, const VisitedNode* rhs)
	{
		return lhs->total_distance > rhs->total_distance;
	}


	struct PathNotFoundException : std::exception
	{
		const char* what() const noexcept override
		{
			return "Failed to find a path!";
		}
	};


	Path* FindPathInternal(Node* start, Node* target)
	{
		if (start == target || start->isSetForDeletion() || target->isSetForDeletion())
		{
			auto result = new Path(0.0f);
			result->addNodeToEnd(start);
			return result;
		}

		std::priority_queue<VisitedNode*, std::vector<VisitedNode*>, std::function<bool(VisitedNode*, VisitedNode*)>> queue(NodeComparator);
		std::vector<VisitedNode> existing_paths(Container::GetHighestNodeID() + 1);
		VisitedNode* solution = nullptr;

		const auto first_node = &existing_paths.at(start->getID());
		first_node->distance = 0.0f;
		first_node->total_distance = first_node->target_distance = start->getDistanceFromNode(target);
		first_node->previous = nullptr;
		first_node->node = start;
		queue.push(first_node);

		while (!queue.empty())
		{
			const auto current_node = queue.top();
			queue.pop();

			if (solution != nullptr && NodeComparator(current_node, solution))
			{
				break;
			}

			for (auto connection : current_node->node->getOutgoingConnections())
			{
				const auto connection_target = connection->getTarget();

				if (connection_target->isSetForDeletion())
				{
					continue;
				}

				auto next_node = &existing_paths.at(connection_target->getID());
				const auto distance = current_node->distance + connection->getDistance();

				if (next_node->distance > distance)
				{
					if (next_node->distance == infinity)
					{
						next_node->node = connection_target;
						next_node->target_distance = target->getDistanceFromNode(connection_target);

						if (connection_target == target)
						{
							solution = next_node;
						}
					}

					next_node->distance = distance;
					next_node->previous = current_node;
					next_node->total_distance = next_node->target_distance + distance;

					queue.push(next_node);
				}
			}
		}

		if (solution == nullptr)
		{
			throw PathNotFoundException();
		}

		auto result = new Path(solution->distance);

		while (solution != nullptr)
		{
			result->addNodeToStart(solution->node);
			solution = solution->previous;
		}

		auto index = 0;

		for (auto node : result->getNodes())
		{
			node->addToPath(result, index++);
		}

		return result;
	}


	int FindPath(Node* start, Node* target)
	{
		try
		{
			return Container::AddPath(FindPathInternal(start, target));
		}
		catch (const PathNotFoundException &e)
		{
			return INVALID_PATH_ID;
		}
	}


	void FindPathThreaded(Node* start, Node* target, Callback* callback)
	{
		Container::LockShared();

		try
		{
			callback->setResult(Container::AddPath(FindPathInternal(start, target)));
		}
		catch (const PathNotFoundException &e)
		{
			callback->setResult(INVALID_PATH_ID);
		}

		Container::UnlockShared();
		amx::QueueCallback(callback->getAMX(), callback);
	}
};
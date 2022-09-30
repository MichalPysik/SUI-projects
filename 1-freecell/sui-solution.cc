#include "search-strategies.h"

#include "memusage.h"

#include <queue>
#include <algorithm>



/*
class Bfs_node {
	public:
		std::shared_ptr<SearchState> parent_state;
		SearchAction action;

		Bfs_node(std::shared_ptr<SearchState> parent_state, SearchAction action) {
			parent_state = parent_state;
			action = SearchAction(action);
		}
};
*/

typedef struct bfs_node {
	std::shared_ptr<SearchState> parent_state;
	std::shared_ptr<SearchAction> action;
} Bfs_node;

std::vector<SearchAction> BreadthFirstSearch::solve(const SearchState &init_state) {
	std::vector<SearchAction> solution, actions;
	std::shared_ptr<SearchState> working_state = std::make_shared<SearchState>(init_state);
	std::shared_ptr<SearchState> tmp_state;

	// queue for storing expanded nodes
	std::queue<std::shared_ptr<SearchState>> open;
	open.push(working_state);

	// map to keep track of expanded states, their parent states and last action leading to them
	std::map<SearchState, Bfs_node> closed;

	closed[*working_state].parent_state = nullptr;
	
	while(!open.empty()) {
		// check whether 1KiB close to memory limit
		if (getCurrentRSS() + 1024 > mem_limit_)
			break;

		working_state = open.front();
		open.pop();

		// check the moves available from the current state
		actions = working_state->actions();

		// if the current node can be expanded...
		if (actions.size() > 0) {
			// ...then do so and insert all possible moves into queue and create a new solution
			for (auto action : actions) {
				tmp_state = std::make_shared<SearchState>(action.execute(*working_state));

				// expanded node leads to some already visited state
				if (closed.count(*tmp_state))
					continue;

				closed[*tmp_state].parent_state = working_state;
				closed[*tmp_state].action = std::make_shared<SearchAction>(action);

				// correct solution found
				if (tmp_state->isFinal()) {
					// create reversed solution and then reverse it into chronological order
					while(closed[*tmp_state].parent_state != nullptr) {
						solution.push_back(*closed[*tmp_state].action);
						tmp_state = closed[*tmp_state].parent_state;
					}
					std::reverse(solution.begin(), solution.end());

					return solution;
				}

				open.push(tmp_state);
			}
		}
	}
	
	return {};
}

typedef struct dfs_node {
	std::shared_ptr<SearchState> parent_state;
	std::shared_ptr<SearchAction> action;
	unsigned long depth;
} Dfs_node;

std::vector<SearchAction> DepthFirstSearch::solve(const SearchState &init_state) {
	std::vector<SearchAction> solution, actions;
	std::shared_ptr<SearchState> working_state = std::make_shared<SearchState>(init_state);
	std::shared_ptr<SearchState> tmp_state;

	// queue for storing expanded nodes
	std::vector<std::shared_ptr<SearchState>> open;
	open.push_back(working_state);

	// map to keep track of expanded states, their parent states and last action leading to them
	std::map<SearchState, Dfs_node> closed;

	closed[*working_state].parent_state = nullptr;
	closed[*working_state].depth = 0;

	while(!open.empty()) {
		if (getCurrentRSS() + 1024 > mem_limit_)
			break;

		working_state = open.back();
		open.pop_back();

		if (closed[*working_state].depth == depth_limit_)
			continue;

		// check the moves available from the current state
		actions = working_state->actions();

		// if the current node can be expanded...
		if (actions.size() > 0) {
			// ...then do so and insert all possible moves into queue and create a new solution
			for (auto action : actions) {
				tmp_state = std::make_shared<SearchState>(action.execute(*working_state));

				// expanded node leads to some already visited state
				if (closed.count(*tmp_state))
					continue;

				closed[*tmp_state].parent_state = working_state;
				closed[*tmp_state].action = std::make_shared<SearchAction>(action);
				closed[*tmp_state].depth = closed[*working_state].depth + 1;

				if (tmp_state->isFinal()) {
					// create reversed solution and then reverse it into chronological order
					while(closed[*tmp_state].parent_state != nullptr) {
						solution.push_back(*closed[*tmp_state].action);
						tmp_state = closed[*tmp_state].parent_state;
					}
					std::reverse(solution.begin(), solution.end());

					return solution;
				}

				open.push_back(tmp_state);
			}
		}
	}
	
	return {};
}

double StudentHeuristic::distanceLowerBound(const GameState &state) const {
    return 0;
}

std::vector<SearchAction> AStarSearch::solve(const SearchState &init_state) {
	return {};
}

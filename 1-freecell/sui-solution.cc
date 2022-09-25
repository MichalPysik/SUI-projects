#include "search-strategies.h"

#include "memusage.h"

#include <queue>


std::vector<SearchAction> BreadthFirstSearch::solve(const SearchState &init_state) {
	std::vector<SearchAction> solution, actions;
	std::shared_ptr<SearchState> working_state(new SearchState(init_state));
	std::shared_ptr<SearchState> tmp_state;

	// queue for storing expanded nodes
	std::queue<std::shared_ptr<SearchState>> state_queue;
	state_queue.push(working_state);

	std::shared_ptr<SearchAction> tmp_action;

	// map that maps solutions to game states
	std::map<SearchState, std::vector<std::shared_ptr<SearchAction>>> solution_map;
	std::vector<std::shared_ptr<SearchAction>> tmp_solution;
	solution_map[*working_state] = tmp_solution;

	while(!state_queue.empty()) {
		// check whether 1KiB close to memory limit
		if (getCurrentRSS() + 1024 > mem_limit_)
			break;

		working_state = state_queue.front();
		state_queue.pop();

		// correct solution found
		if (working_state->isFinal()) {
			tmp_solution = solution_map[*working_state];

			// convert correct solution from vector of pointers to vector of objects
			SearchAction act(*tmp_solution.back());
			solution.push_back(act);
			tmp_solution.pop_back();

			while(!tmp_solution.empty()) {
				act = *tmp_solution.front();
				solution.push_back(act);
				tmp_solution.erase(tmp_solution.begin());
			}

			return solution;
		}

		// check the moves available from the current state
		actions = working_state->actions();

		// if the current node can be expanded...
		if (actions.size() > 0) {
			// ...then do so and insert all possible moves into queue and create a new solution
			for (auto action : actions) {

				tmp_state = std::make_shared<SearchState>(*working_state);
				*tmp_state = action.execute(*tmp_state);

				// expanded node leads to some already visited state
				if (solution_map.count(*tmp_state))
					continue;

				tmp_action = std::make_shared<SearchAction>(action);
				tmp_solution = solution_map[*working_state];
				tmp_solution.push_back(tmp_action);
				solution_map[*tmp_state] = tmp_solution;

				state_queue.push(tmp_state);
			}
		}
		
		// delete the current state's associated solution,
		// but keep the state to remember that it was visited
		solution_map[*working_state].clear();
	}

	return {};
}

std::vector<SearchAction> DepthFirstSearch::solve(const SearchState &init_state) {
	std::vector<SearchAction> solution, actions;
	SearchState working_state(init_state);
	SearchState tmp_state(working_state);

	// stack for storing current expanded nodes
	std::vector<SearchState> state_stack;
	state_stack.push_back(working_state);

	// temporary just for variable declaration
	Location a, b;
	a.cl = b.cl = LocationClass::Homes;
	a.id = b.id = 0;
	SearchAction last_action(a, b);
	bool action_done;

	std::map<SearchState, char> state_memory;

	while(!state_stack.empty()) {
		working_state = SearchState(state_stack.back());
		state_stack.pop_back();
		state_memory[working_state] = '\0';

		if (working_state.isFinal())
			return solution;

		if (solution.size() == depth_limit_)
			continue;

		// check the moves available from the current state
		actions = working_state.actions();

		// if the current node can be expanded...
		if (actions.size() > 0) {
			// ...then do so and insert all possible moves into queue and create a new solution
			action_done = false;
			for (auto action : actions) {
				// check whether enough memory is still available for the move to execute
				if (getCurrentRSS() + 2*(sizeof(SearchState) + sizeof(CardStorage) + 4) + sizeof(std::vector<RawMove>) > mem_limit_)
					return {};

				tmp_state = action.execute(working_state);

				// expanded node leads to some already visited state
				if (state_memory.count(tmp_state))
					continue;

				state_stack.push_back(tmp_state);
				last_action = action;
				action_done = true;
			}
			if (action_done)
				solution.push_back(last_action);
		}
		else
			solution.pop_back();
	}

	return {};
}

double StudentHeuristic::distanceLowerBound(const GameState &state) const {
    return 0;
}

std::vector<SearchAction> AStarSearch::solve(const SearchState &init_state) {
	return {};
}

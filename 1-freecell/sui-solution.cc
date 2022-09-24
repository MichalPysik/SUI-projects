#include "search-strategies.h"
#include <queue>

std::vector<SearchAction> BreadthFirstSearch::solve(const SearchState &init_state) {
	std::vector<SearchAction> solution;
	SearchState *working_state = new SearchState(init_state);
	SearchState *tmp_state;

	// queue for storing expanded nodes
	std::queue<SearchState*> state_queue;
	state_queue.push(working_state);

	// vector that stores all actions for final deletion
	std::vector<SearchAction*> action_bank;
	SearchAction *tmp_action;

	// map that maps solutions to game states
	std::map<SearchState, std::vector<SearchAction*>> solution_map;
	std::vector<SearchAction*> tmp_solution;
	solution_map[*working_state] = tmp_solution;

	while(!state_queue.empty()) {
		working_state = state_queue.front();

		// correct solution found
		if (working_state->isFinal()) {
			tmp_solution = solution_map[*working_state];
			solution_map.erase(*working_state);

			/* if the game was won by default, break the cycle and return empty solution
			if(tmp_solution.empty()) {
				delete working_state;
				break;
			} */

			// convert correct solution from vector of pointers to vector of objects
			SearchAction act(*tmp_solution.back());
			solution.push_back(act);
			tmp_solution.pop_back();
			while(!tmp_solution.empty()) {
				act = *tmp_solution.front();
				solution.push_back(act);
				tmp_solution.erase(tmp_solution.begin());
			}

			// clear the queue of remaining states and their associated solutions
			while(!state_queue.empty()) {
				working_state = state_queue.front();
				state_queue.pop();
				solution_map.erase(*working_state);
				delete working_state;
			}
			
			// delete all actions from the action bank
			while (!action_bank.empty()) {
				delete action_bank.back();
				action_bank.pop_back();
			}

			return solution;
		}

		state_queue.pop();

		// check the moves available from the current state
		auto actions = working_state->actions();

		// if the current node can be expanded...
		if (actions.size() > 0) {
			// ...then do so and insert all possible moves into queue and create a new solution
			for (auto action : actions) {
				tmp_state = new SearchState(*working_state);
				*tmp_state = action.execute(*tmp_state);

				// expanded node leads to some already visited state
				if (solution_map.count(*tmp_state)) {
					delete tmp_state;
					continue;
				}

				tmp_action = new SearchAction(action);
				action_bank.push_back(tmp_action);
				tmp_solution = solution_map[*working_state];
				tmp_solution.push_back(tmp_action);
				solution_map[*tmp_state] = tmp_solution;

				state_queue.push(tmp_state);
			}
		}
		
		// delete the current state and its associated solution
		solution_map.erase(*working_state);
		delete working_state;
	}

	return {};
}

std::vector<SearchAction> DepthFirstSearch::solve(const SearchState &init_state) {
	return {};
}

double StudentHeuristic::distanceLowerBound(const GameState &state) const {
    return 0;
}

std::vector<SearchAction> AStarSearch::solve(const SearchState &init_state) {
	return {};
}

#include "search.h"
#include "stack.h"
#include <stdlib.h>
#include <math.h>
#include "heuristic.h"
#include <stdio.h>

/** Lista de operaciones */
static Operation* ops;
/** Cantidad total de operaciones */
static uint16_t op_count;
/** Estado solución */
static Puzzle* goal;

/** Looks for a solution of cost no more than "limit" */
bool search_limited_depth(Puzzle* state, double cost, double limit, Stack* s, double* next)
{
	/** If I'm at the solution, I'm done */
	if(puzzle_equals(state, goal)) return true;

	/* Refine own heuristic based on son's heuristics, for next pass */
	double cheapest_son_heuristic = INFINITY;

	/* Try all operations in order */
	for(int i = 0; i < op_count; i++)
	{
		/* Generate next state */
		operation_execute(state, ops[i]);

		/* Save operation that did this */
		stack_push(s, ops[i]);

		// TODO call table and obtain if state should be explored and it's heuristic
		bool should_expand = true;
		/* Compute son's heuristic or TODO retrieve it from the table */
		double son_heuristic = heuristic_full(state);

		if(should_expand)
		{
			/* Optimistic distance to the solution */
			double son_cost = cost + 1 + son_heuristic;

			/* Don't bother calling the son if it can't afford it */
			if(son_cost > limit)
		 	{
				/* Next bound will be the lowest that wasn't explored*/
				if(son_cost < *next)
				{
					*next = son_cost;
				}
			}
			else
			{
				/* Expand son */
				if(search_limited_depth(state, cost + 1, limit, s, next))
				{
					return true;
				}
			}
		}

		/* Undo operation, go back to current state */
		operation_revert(state, stack_pop(s));

		/* Compare this son's heuristic with its brothers' */
		cheapest_son_heuristic = fmin(cheapest_son_heuristic, son_heuristic);
	}

	//TODO update heuristic at table
	// double my_better_heuristic = cheapest_son_heuristic + 1;

	return false;
}

/** Looks for a sequence of steps to get from state to goal */
Stack* search(Puzzle* state, Puzzle* final)
{
	ops = operation_generate_all(state);
	goal = final;
	op_count = 2 * (state -> height + state -> width);

	heuristic_init(final);

	Stack* stack = stack_init();

	double limit = 0;

	double next_limit;

	while(true)
	{
		next_limit = INFINITY;
		if(search_limited_depth(state, 0, limit, stack, &next_limit))
		{
			free(ops);
			heuristic_free(final);
			return stack;
		}
		else
		{
			limit = next_limit;
		}
	}
	abort();
}

#include "search.h"
#include "stack.h"
#include <stdlib.h>
#include <math.h>
#include "heuristic.h"

/** Lista de operaciones */
static Operation* ops;
/** Cantidad total de operaciones */
static uint16_t op_count;
/** Estado solución */
static Puzzle* goal;

/** Looks for a solution of cost no more than "limit" */
bool search_limited_depth(Puzzle* state, double cost, double limit, Stack* s, double* next)
{
	if(cost > limit)
 	{
		if(cost < *next)
		{
			*next = cost;
		}
		return false;
	}

	if(puzzle_equals(state, goal)) return true;

	for(int i = 0; i < op_count; i++)
	{
		operation_execute(state, ops[i]);

		stack_push(s, ops[i]);

		if(search_limited_depth(state, cost + 1 + heuristic_full(state), limit, s, next))
		{
			return true;
		}

		operation_revert(state, stack_pop(s));
	}

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

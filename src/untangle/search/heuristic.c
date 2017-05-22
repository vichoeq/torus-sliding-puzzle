#include "heuristic.h"
#include <stdlib.h>
#include <math.h>

/** Distance to closest end position for each cell for each color */
static uint8_t*** closest;

typedef struct
{
	uint8_t row;
	uint8_t col;
} Cell;

static uint8_t manhattan_torus_distance(Puzzle* puz, Cell c1, Cell c2)
{
	uint8_t r = c1.row;
	uint8_t c = c1.col;

	uint8_t i = c2.row;
	uint8_t j = c2.col;

	uint8_t m = puz -> width;
	uint8_t n = puz -> height;
	/* Trusting */
	// http://stackoverflow.com/questions/29781359/how-to-find-manhattan-distance-in-a-continuous-two-dimensional-matrix
	return fmin(abs(r-i), n-1-abs(r-i))  +  fmin(abs(c-j), m-1-abs(c-j));
}

/** Computes the closest distance between this cell and a cell of color k */
static uint8_t closest_distance(Puzzle* puz, int k, Cell c)
{
	uint8_t closest = UINT8_MAX;

	for(uint8_t row = 0; row < puz -> height; row++)
	{
		for(uint8_t col = 0; col < puz -> width; col++)
		{
			if(puz -> matrix[row][col] == k)
			{
				Cell candidate = (Cell){.row = row, .col = col};

				uint8_t distance = manhattan_torus_distance(puz, c, candidate);

				if(distance < closest)
				{
					closest = distance;
				}
			}
		}
	}
	return closest;
}

/** Precomputes the heuristic to allow incremental calculations */
void heuristic_init(Puzzle* solution)
{
	closest = calloc(8, sizeof(uint8_t**));

	for(int k = 0; k < 8; k++)
	{
		closest[k] = calloc(solution -> height, sizeof(uint8_t*));

		for(uint8_t row = 0; row < solution -> height; row++)
		{
			closest[k][row] = calloc(solution -> width, sizeof(uint8_t));

			for(uint8_t col = 0; col < solution -> width; col++)
			{
				Cell c = (Cell){.row = row, .col = col};

				closest[k][row][col] = closest_distance(solution, k, c);
			}
		}
	}
}

/** Fully computes the heuristic of a given state */
double heuristic_full(Puzzle* state)
{
	double bound = fmax(state -> width, state -> height);

	double to_fix = 0;

	for(uint8_t row = 0; row < state -> height; row++)
	{
		for(uint8_t col = 0; col < state -> width; col++)
		{
			uint8_t k = state -> matrix[row][col];

			to_fix += closest[k][row][col];
		}
	}
	return to_fix / bound;
}

/** Gets the heuristic of a state given the heuristic of the current one */
double heuristic_next(Puzzle* state, double heuristic, Operation op);

/** Releases all the resources used up by this module */
void   heuristic_free(Puzzle* solution)
{
	for(int k = 0; k < 8; k++)
	{
		for(uint8_t row = 0; row < solution -> height; row++)
		{
			free(closest[k][row]);
		}
		free(closest[k]);
	}
	free(closest);
}

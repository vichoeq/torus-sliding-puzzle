#pragma once

#include "../../puzzle/operation.h"

/** Precomputes the heuristic to allow incremental calculations */
void   heuristic_init(Puzzle* solution);
/** Fully computes the heuristic of a given state */
double heuristic_full(Puzzle* state);
/** Gets the heuristic of a state given the heuristic of the current one */
double heuristic_next(Puzzle* state, double heuristic, Operation op);
/** Releases all the resources used up by this module */
void   heuristic_free(Puzzle* solution);

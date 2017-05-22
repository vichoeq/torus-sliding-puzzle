#pragma once

#include "stack.h"

/** Looks for a sequence of steps to get from state to goal */
Stack* search(Puzzle* state, Puzzle* final);

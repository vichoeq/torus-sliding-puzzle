#pragma once

#include "color.h"
#include "../kdtree/kdtree.h"

/** Finds the median of a given set of colors in a specific channel */
Color find_median(Color* colors, int length, Axis axis);
/** Compare two colors on a given channel */
int compare(Color c1, Color c2, Axis axis);

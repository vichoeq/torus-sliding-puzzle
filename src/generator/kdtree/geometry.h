#pragma once

#include <stdbool.h>
#include "../color/color.h"

/** Represents an axis aligned bounding box thightly enveloping some points */
struct axis_aligned_bounding_box
{
	/** Color containing the lesser bounds on each channel */
	Color min;
	/** Color containing the greater bounds on each channel */
	Color max;
};

/** Represents an axis aligned bounding box enveloping a set of points */
typedef struct axis_aligned_bounding_box AABB;

/** Indicates whether a given sphere collides with a box or not */
bool aabb_collides_sphere (AABB box, Color center, double radius);
/** Builds the thightest box containing all the given points */
AABB aabb_build           (Color* colors, int length);
/** Expands the box so the point can be in it */
AABB aabb_add_point       (AABB box, Color c);
/** Creates the thightest box containing the two given boxes */
AABB aabb_join_aabb       (AABB box1, AABB box2);
/** Creates a box big enough to contain a single point */
AABB aabb_from_point      (Color c);
/** Creates an empty box */
AABB aabb_empty           ();

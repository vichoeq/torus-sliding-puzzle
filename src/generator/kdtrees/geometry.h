#ifndef T1_SOL_GEOMETRY
#define T1_SOL_GEOMETRY

#include "../imagelib/imagelib.h"
#include <stdbool.h>

typedef struct axis_aligned_bounding_box
{
	Color min;
	Color max;
} AABB;

Axis get_splitting_axis(AABB box);

bool aabb_collides_sphere(AABB box, Color center, double radius);

AABB aabb_find(Color* colors, int length);

AABB aabb_add_point(AABB box, Color c);

AABB aabb_join_aabb(AABB box1, AABB box2);

AABB aabb_from_point(Color c);

AABB aabb_empty();

#endif /* end of include guard: T1_SOL_GEOMETRY */

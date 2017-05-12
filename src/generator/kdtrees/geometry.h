#ifndef T1_SOL_GEOMETRY
#define T1_SOL_GEOMETRY

#include "../imagelib/imagelib.h"
#include <stdbool.h>

typedef struct axis_aligned_bounding_box
{
	Color_lab min;
	Color_lab max;
} AABB;

double euclidean_distance(Color_lab p1, Color_lab p2);

double euclidean_distance_squared(Color_lab p1, Color_lab p2);

bool   aabb_collides_sphere(AABB box, Color_lab center, double radius);

AABB aabb_find(Color_lab* colors, int length);

AABB aabb_add_point(AABB box, Color_lab c);

AABB aabb_join_aabb(AABB box1, AABB box2);

AABB aabb_from_point(Color_lab c);

AABB aabb_empty();

#endif /* end of include guard: T1_SOL_GEOMETRY */

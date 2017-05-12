#include "geometry.h"
#include <math.h>

double euclidean_distance(Color_lab p1, Color_lab p2)
{
  return sqrt(euclidean_distance_squared(p1, p2));
}

double euclidean_distance_squared(Color_lab p1, Color_lab p2)
{
	return pow(p2.L - p1.L, 2) + pow(p2.a - p1.a, 2) + pow(p2.b - p1.b, 2);
}

bool aabb_collides_sphere(AABB box, Color_lab center, double radius)
{
	Color_lab closest;

	// get box closest point to sphere center by clamping
	closest.L = fmaxf(box.min.L, fminf(center.L, box.max.L));
	closest.a = fmaxf(box.min.a, fminf(center.a, box.max.a));
	closest.b = fmaxf(box.min.b, fminf(center.b, box.max.b));

	return euclidean_distance(closest, center) < radius;
}

AABB aabb_empty()
{
	AABB box;

	box.max.L = -INFINITY;
	box.max.a = -INFINITY;
	box.max.b = -INFINITY;

	box.min.L = INFINITY;
	box.min.a = INFINITY;
	box.min.b = INFINITY;

	return box;
}

AABB aabb_add_point(AABB box, Color_lab c)
{
	box.max.L = fmax(box.max.L, c.L);
	box.max.a = fmax(box.max.a, c.a);
	box.max.b = fmax(box.max.b, c.b);

	box.min.L = fmin(box.min.L, c.L);
	box.min.a = fmin(box.min.a, c.a);
	box.min.b = fmin(box.min.b, c.b);

	return box;
}

AABB aabb_join_aabb(AABB box1, AABB box2)
{
	AABB box;

	box.min.L = fminf(box1.min.L, box2.min.L);
	box.min.a = fminf(box1.min.a, box2.min.a);
	box.min.b = fminf(box1.min.b, box2.min.b);

	box.max.L = fmaxf(box1.max.L, box2.max.L);
	box.max.a = fmaxf(box1.max.a, box2.max.a);
	box.max.b = fmaxf(box1.max.b, box2.max.b);

	return box;
}

AABB aabb_from_point(Color_lab c)
{
	AABB box;

	box.min.L = c.L;
	box.min.a = c.a;
	box.min.b = c.b;

	box.max.L = c.L;
	box.max.a = c.a;
	box.max.b = c.b;

	return box;
}

AABB aabb_find(Color_lab* colors, int length)
{
	AABB box = aabb_empty();

	for(int i = 0; i < length; i++)
	{
		aabb_add_point(box, colors[i]);
	}

	return box;
}

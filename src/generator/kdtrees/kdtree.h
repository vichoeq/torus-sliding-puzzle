#ifndef T2_SOL_KDTREE
#define T2_SOL_KDTREE

#include "../imagelib/imagelib.h"
#include "geometry.h"

typedef enum axis
{
	L,
	a,
	b
} Axis;

struct kdtree;
typedef struct kdtree KDTree;

struct kdtree
{
	Axis split;
	bool isLeaf;
	KDTree* left_son;
	KDTree* right_son;
	Color_lab color;
	KDTree* parent;
	KDTree* brother;
	AABB box;
};

KDTree*    kd_create(Color_lab* colors, int length, int k);

Color_lab  kd_find_closest(KDTree* kd, Color_lab color);

Color_lab  kd_find_closest_greedy(KDTree* kd, Color_lab color);

void       kd_destroy(KDTree* kd);

#endif /* end of include guard: T2_SOL_KDTREE */

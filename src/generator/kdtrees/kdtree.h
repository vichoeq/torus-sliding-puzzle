#ifndef T2_SOL_KDTREE
#define T2_SOL_KDTREE

#include "../imagelib/imagelib.h"
#include "geometry.h"

struct kdtree;
typedef struct kdtree KDTree;

struct kdtree
{
	Axis split;
	bool isLeaf;
	KDTree* left_son;
	KDTree* right_son;
	Color color;
	KDTree* parent;
	KDTree* brother;
	AABB box;
};

KDTree*    kd_create(Color* colors, int length, int k);

Color  kd_find_closest(KDTree* kd, Color color);

void       kd_destroy(KDTree* kd);

#endif /* end of include guard: T2_SOL_KDTREE */

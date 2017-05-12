#pragma once

#include "geometry.h"

/** Represents the orientation of a plane dividing the space in two */
enum axis
{
	/** Lightness channel */
	L,
	/** Magenta / Green channel */
	a,
	/** Blue / Yellow channel */
	b
};
/** Represents the orientation of a plane dividing the space in two */
typedef enum axis Axis;

/** 3-Dimensional KD-Tree for a color palette. Leaves are the colors */
struct kd_node;
/** 3-Dimensional KD-Tree for a color palette. Leaves are the colors */
typedef struct kd_node KDTree;

struct kd_node
{
	/** Orientation of the plane cutting the tree in half */
	Axis split;
	/* Whether this node is a leaf */
	bool isLeaf;
	/** Tree with points less or equal than the cutting plane */
	KDTree* left_son;
	/** Tree with points greater than the cutting plane */
	KDTree* right_son;
	/** Position of the cutting plane, or accumulated color if it's a leaf */
	Color color;
	/** Parent of this tree */
	KDTree* parent;
	/** Brother of this tree */
	KDTree* brother;
	/** Thight box enveloping all the points on this tree */
	AABB box;
};

/** Builds the whole tree from a given set of colors */
KDTree* kd_create       (Color* colors, int length, int k);
/** Looks for the leaf with the color closest to the given one */
Color   kd_find_closest (KDTree* kd, Color color);
/** Frees the resources used up by this tree */
void    kd_destroy      (KDTree* kd);

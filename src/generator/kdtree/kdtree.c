#include "kdtree.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../color/sorting.h"

/** Decides which axis would be best to divide this box */
Axis get_splitting_axis(AABB box)
{
	/* The channel with the biggest span will be the next one to be divided */
	float DL = box.max.L - box.min.L;
	float Da = box.max.a - box.min.a;
	float Db = box.max.b - box.min.b;

	if(Da <= Db)
	{
		if(Db <= DL)
		{
			return L;
		}
		return b;
	}
	if(Da <= DL)
	{
		return L;
	}
	return a;
}

/** Recursive building of the tree */
static void kd_recursive(KDTree* node, Color* colors, int length, int k, int depth, AABB box)
{
	/* If we've reached the limit depth, then we are in a leaf */
	if(k == depth)
	{
		node -> isLeaf = true;
		node -> right_son = NULL;
		node -> left_son = NULL;
		node -> color = color_average(colors, length);
		node -> box = aabb_from_point(node -> color);
	}
	else
	{
		Axis ax = get_splitting_axis(box);

		Color median = find_median(colors, length, ax);

		node -> color = median;
		node -> split = ax;
		node -> isLeaf = false;

		Color* less_or_equal = calloc(length, sizeof(Color));
		AABB less_or_equal_box = aabb_empty();
		int less_or_equal_count = 0;

		Color* greater = calloc(length, sizeof(Color));
		AABB greater_box = aabb_empty();
		int greater_count = 0;

		for(int i = 0; i < length; i++)
		{
			if(compare(colors[i], median, ax) <= 0)
			{
				less_or_equal[less_or_equal_count++] = colors[i];
				less_or_equal_box = aabb_add_point(less_or_equal_box, colors[i]);
			}
			else
			{
				greater[greater_count++] = colors[i];
				greater_box = aabb_add_point(greater_box, colors[i]);
			}
		}

		node -> left_son = malloc(sizeof(KDTree));
		node -> left_son -> parent = node;
		node -> right_son = malloc(sizeof(KDTree));
		node -> right_son -> parent = node;
		node -> left_son -> brother = node -> right_son;
		node -> right_son -> brother = node -> left_son;


		kd_recursive(node -> left_son, less_or_equal, less_or_equal_count, k, depth + 1, less_or_equal_box);
		kd_recursive(node -> right_son, greater, greater_count, k, depth + 1, greater_box);

		node -> box = aabb_join_aabb(node -> left_son -> box, node -> right_son -> box);
	}

	free(colors);
}

/** Builds the whole tree from a given set of colors */
KDTree* kd_create(Color* colors, int length, int k)
{
	KDTree* root = malloc(sizeof(KDTree));
	root -> parent = NULL;
	root -> brother = NULL;
	kd_recursive(root, colors, length, k, 0, aabb_build(colors, length));
	return root;
}

/** Recursively looks for the leaf where this would have gone */
static KDTree* kd_find_corresponding(KDTree* kd, Color color)
{
	if(kd -> isLeaf)
	{
		return kd;
	}
	else
	{
		if(compare(color, kd -> color, kd -> split) <= 0)
		{
			return kd_find_corresponding(kd -> left_son, color);
		}
		else
		{
			return kd_find_corresponding(kd -> right_son, color);
		}
	}
}

/** Looks for the leaf with the color closest to the given one */
Color kd_find_closest(KDTree* kd, Color color)
{
	KDTree* current = kd_find_corresponding(kd, color);

	double radius = euclidean_distance(color, current -> color);
	Color closest = current -> color;

	/* We go up from the leaves until we reach the root */
	while(current -> parent)
	{
		/* There could be a point closer than the closest i've seen */
		if(aabb_collides_sphere(current -> brother -> box, color, radius))
		{
			/* Cut the link so the search doesn't go higher than here */
			current -> brother -> parent = NULL;

			/* Sweeps the brother tree bottom up */
			Color candidate = kd_find_closest(current -> brother, color);
			double distance = euclidean_distance(color, candidate);
			if(distance < radius)
			{
				closest = candidate;
				radius = distance;
			}
			/* Link the trees back */
			current -> brother -> parent = current -> parent;
		}
		/* Go up one level */
		current = current -> parent;
	}
	return closest;
}

/** Frees the resources used up by this tree */
void kd_destroy(KDTree* kd)
{
	if(kd -> isLeaf)
	{
		free(kd);
	}
	else
	{
		kd_destroy(kd -> left_son);
		kd_destroy(kd -> right_son);
		free(kd);
	}
}

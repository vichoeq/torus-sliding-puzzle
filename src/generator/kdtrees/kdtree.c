#include "kdtree.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../color/sorting.h"

static void kd_recursive(KDTree* node, Color* colors, int length, int k, int depth, AABB box)
{
	/* Llegamos a una hoja */
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

		Color* less_or_equal = malloc(sizeof(Color) * length);
		Color* greater = malloc(sizeof(Color) * length);
		int less_or_equal_count = 0;
		int greater_count = 0;
		AABB less_or_equal_box = aabb_empty();
		AABB greater_box = aabb_empty();

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

KDTree* kd_create(Color* colors, int length, int k)
{
	KDTree* root = malloc(sizeof(KDTree));
	root -> parent = NULL;
	root -> brother = NULL;
	kd_recursive(root, colors, length, k, 0, aabb_find(colors, length));
	return root;
}

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

Color kd_find_closest(KDTree* kd, Color color)
{
	KDTree* current = kd_find_corresponding(kd, color);

	double radius = euclidean_distance(color, current -> color);
	Color closest = current -> color;

	while(current -> parent)
	{
		if(aabb_collides_sphere(current -> brother -> box, color, radius))
		{
			current -> brother -> parent = NULL;
			Color candidate = kd_find_closest(current -> brother, color);
			double distance = euclidean_distance(color, candidate);
			if(distance < radius)
			{
				closest = candidate;
				radius = distance;
			}
			current -> brother -> parent = current -> parent;
		}

		current = current -> parent;
	}
	return closest;
}

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

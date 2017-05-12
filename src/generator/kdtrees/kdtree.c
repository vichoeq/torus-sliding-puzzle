#include "kdtree.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

Axis get_splitting_axis(AABB box)
{
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

Color_lab color_average(Color_lab* colors, int length)
{
	double L = 0;
	double a = 0;
	double b = 0;

	for(int i = 0; i < length; i++)
	{
		L += colors[i].L;
		a += colors[i].a;
		b += colors[i].b;
	}

	Color_lab result;
	result.L = L / (double)length;
	result.a = a / (double)length;
	result.b = b / (double)length;

	return result;
}

int compare_L(const void* p1, const void* p2)
{
	Color_lab* c1 = (Color_lab*) p1;
	Color_lab* c2 = (Color_lab*) p2;
	if(c1 -> L < c2 -> L) return -1;
	else if(c1 -> L > c2 -> L) return 1;
	else return 0;
}

int compare_a(const void* p1, const void* p2)
{
	Color_lab* c1 = (Color_lab*) p1;
	Color_lab* c2 = (Color_lab*) p2;
	if(c1 -> a < c2 -> a) return -1;
	else if(c1 -> a > c2 -> a) return 1;
	else return 0;
}

int compare_b(const void* p1, const void* p2)
{
	Color_lab* c1 = (Color_lab*) p1;
	Color_lab* c2 = (Color_lab*) p2;
	if(c1 -> b < c2 -> b) return -1;
	else if(c1 -> b > c2 -> b) return 1;
	else return 0;
}

int i_compare_L(Color_lab c1, Color_lab c2)
{
	if(c1.L < c2.L) return -1;
	else if(c1.L > c2.L) return 1;
	else return 0;
}

int i_compare_a(Color_lab c1, Color_lab c2)
{
	if(c1.a < c2.a) return -1;
	else if(c1.a > c2.a) return 1;
	else return 0;
}

int i_compare_b(Color_lab c1, Color_lab c2)
{
	if(c1.b < c2.b) return -1;
	else if(c1.b > c2.b) return 1;
	else return 0;
}

int compare(const void* p1, const void* p2, Axis ax)
{
	switch(ax)
	{
		case L: return compare_L(p1,p2);
		case a: return compare_a(p1,p2);
		case b: return compare_b(p1,p2);
	}
	abort();
}

void swap(Color_lab* array, int i, int j)
{
	Color_lab aux = array[i];
	array[i] = array[j];
	array[j] = aux;
}

int partition(Color_lab* a, int e, int w, int (*compare)(Color_lab, Color_lab))
{
	if(e >= w) abort();
	int j = e-1;
	int k = w;
	Color_lab v = a[w];
	while(true)
	{
		j = j + 1;
		while(j <= w && compare(a[j], v) < 0)
		{
			j = j + 1;
		}
		k = k-1;
		while(k >= e && compare(a[k], v) > 0)
		{
			k = k-1;
		}
		if(j >= k) break;
		swap(a,j,k);


	}
	swap(a,j,w);
	return j;
}

long random_at_most(long max) {
  unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  long x;
  do {
   x = random();
  }
  // This is carefully written not to overflow
  while (num_rand - defect <= (unsigned long)x);

  // Truncated division is intentional
  return x/bin_size;
}

int randomPartition(Color_lab* a, int e, int w, int (*compare)(Color_lab, Color_lab))
{
	int j;
	if(w-e > 0)
	{
		j = random_at_most(w - e) + e;
	}
	else
	{
		j = w;
	}
	if(j < e) abort();
	if(j > w) abort();
	swap(a, j, w);
	return partition(a, e, w, compare);
}

void quicksort(Color_lab* a, int e, int w, int (*compare)(Color_lab, Color_lab))
{
	if(e < w)
	{
		// printf("Sorting %p[%d,%d]\n", a,e,w);
		int m = randomPartition(a,e,w, compare);
		quicksort(a, e, m-1, compare);
		quicksort(a,m+1,w, compare);
	}
}

/** Ordena parcialmente el arreglo de manera que a[k] queda conteniendo el valor correcto */
void quickselect(Color_lab* a, int e, int w, int k, int (*compare)(Color_lab, Color_lab))
{
	if(e < w)
	{
		// printf("Sorting %p[%d,%d]\n", a,e,w);
		int m = randomPartition(a, e, w, compare);

		if(m > k) return quickselect(a, e, m-1, k, compare);
		if(m < k) return quickselect(a, m+1, w, k, compare);
	}
}

Color_lab find_median(Color_lab* colors, int length, Axis ax)
{
	switch(ax)
	{
		case L: quickselect(colors, 0, length - 1, length / 2, i_compare_L); break;
		case a: quickselect(colors, 0, length - 1, length / 2, i_compare_a); break;
		case b: quickselect(colors, 0, length - 1, length / 2, i_compare_b); break;
	}

	Color_lab median = colors[length / 2];
	if(length % 2 == 0)
	{
		switch(ax)
		{
			case L: quickselect(colors, 0, length / 2 - 1, length / 2 -1, i_compare_L); break;
			case a: quickselect(colors, 0, length / 2 - 1, length / 2 -1, i_compare_a); break;
			case b: quickselect(colors, 0, length / 2 - 1, length / 2 -1, i_compare_b); break;
		}

		median.L = (median.L + colors[length / 2 - 1].L) / 2;
		median.a = (median.a + colors[length / 2 - 1].a) / 2;
		median.b = (median.b + colors[length / 2 - 1].b) / 2;
	}

	return median;
}

void kd_recursive(KDTree* node, Color_lab* colors, int length, int k, int depth, AABB box)
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
		Color_lab median = find_median(colors, length, ax);

		node -> color = median;

		node -> split = ax;
		node -> isLeaf = false;

		Color_lab* less_or_equal = malloc(sizeof(Color_lab) * length);
		Color_lab* greater = malloc(sizeof(Color_lab) * length);
		int less_or_equal_count = 0;
		int greater_count = 0;
		AABB less_or_equal_box = aabb_empty();
		AABB greater_box = aabb_empty();

		for(int i = 0; i < length; i++)
		{
			if(compare(&colors[i], &median, ax) <= 0)
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

KDTree* kd_create(Color_lab* colors, int length, int k)
{
	KDTree* root = malloc(sizeof(KDTree));
	root -> parent = NULL;
	root -> brother = NULL;
	kd_recursive(root, colors, length, k, 0, aabb_find(colors, length));
	return root;
}

KDTree* kd_find_corresponding(KDTree* kd, Color_lab color)
{
	if(kd -> isLeaf)
	{
		return kd;
	}
	else
	{
		if(compare(&color, &kd -> color, kd -> split) <= 0)
		{
			return kd_find_corresponding(kd -> left_son, color);
		}
		else
		{
			return kd_find_corresponding(kd -> right_son, color);
		}
	}
}

Color_lab kd_find_closest_greedy(KDTree* kd, Color_lab color)
{
	KDTree* current = kd_find_corresponding(kd, color);

	return current -> color;
}

Color_lab kd_find_closest(KDTree* kd, Color_lab color)
{
	KDTree* current = kd_find_corresponding(kd, color);

	double radius = euclidean_distance(color, current -> color);
	Color_lab closest = current -> color;

	while(current -> parent)
	{
		if(aabb_collides_sphere(current -> brother -> box, color, radius))
		{
			current -> brother -> parent = NULL;
			Color_lab candidate = kd_find_closest(current -> brother, color);
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

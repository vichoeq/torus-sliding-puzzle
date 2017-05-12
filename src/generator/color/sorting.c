
#include "sorting.h"
#include <stdlib.h>
#include <stdbool.h>

typedef int (*compare_color_fn_t)(Color, Color);

/* http://stackoverflow.com/questions/2509679/how-to-generate-a-random-number-from-within-a-range */
unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

/** Compare two colors on the L channel */
int i_compare_L(Color c1, Color c2)
{
	if(c1.L < c2.L) return -1;
	else if(c1.L > c2.L) return 1;
	else return 0;
}

/** Compare two colors on the a channel */
int i_compare_a(Color c1, Color c2)
{
	if(c1.a < c2.a) return -1;
	else if(c1.a > c2.a) return 1;
	else return 0;
}

/** Compare two colors on the b channel */
int i_compare_b(Color c1, Color c2)
{
	if(c1.b < c2.b) return -1;
	else if(c1.b > c2.b) return 1;
	else return 0;
}

/** Compare two colors on a given channel */
int compare(Color c1, Color c2, Axis ax)
{
	switch(ax)
	{
		case L: return i_compare_L(c1,c2);
		case a: return i_compare_a(c1,c2);
		case b: return i_compare_b(c1,c2);
	}
	abort();
}

/** Swaps the values of two elements in an array */
static void swap(Color* array, int i, int j)
{
	Color aux = array[i];
	array[i] = array[j];
	array[j] = aux;
}

/** Quicksort partition routine */
static int partition(Color* a, int e, int w, compare_color_fn_t compare)
{
	if(e >= w) abort();
	int j = e-1;
	int k = w;
	Color v = a[w];
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

/** Choosing of random pivot for partition */
static int randomPartition(Color* a, int e, int w, compare_color_fn_t compare)
{
	int j;
	if(w - e > 0)
	{
		j = rand_interval(e, w);
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

/** Partially sorts the array so that a[k] ends in its correct position */
static void quickselect(Color* a, int e, int w, int k, compare_color_fn_t compare)
{
	if(e < w)
	{
		int m = randomPartition(a, e, w, compare);

		if(m > k) return quickselect(a, e, m-1, k, compare);
		if(m < k) return quickselect(a, m+1, w, k, compare);
	}
}

/** Finds the median of the array on a given channel */
Color find_median(Color* colors, int length, Axis ax)
{
	switch(ax)
	{
		case L: quickselect(colors, 0, length - 1, length / 2, i_compare_L); break;
		case a: quickselect(colors, 0, length - 1, length / 2, i_compare_a); break;
		case b: quickselect(colors, 0, length - 1, length / 2, i_compare_b); break;
	}

	Color median = colors[length / 2];
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

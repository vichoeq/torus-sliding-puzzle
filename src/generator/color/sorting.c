
#include "sorting.h"
#include <stdlib.h>
#include <stdbool.h>

typedef int (*compare_color_fn_t)(Color, Color);


static void swap(Color* array, int i, int j)
{
	Color aux = array[i];
	array[i] = array[j];
	array[j] = aux;
}

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

static long random_at_most(long max)
{
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

static int randomPartition(Color* a, int e, int w, compare_color_fn_t compare)
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

/** Ordena parcialmente el arreglo de manera que a[k] queda conteniendo el valor correcto */
static void quickselect(Color* a, int e, int w, int k, compare_color_fn_t compare)
{
	if(e < w)
	{
		// printf("Sorting %p[%d,%d]\n", a,e,w);
		int m = randomPartition(a, e, w, compare);

		if(m > k) return quickselect(a, e, m-1, k, compare);
		if(m < k) return quickselect(a, m+1, w, k, compare);
	}
}

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

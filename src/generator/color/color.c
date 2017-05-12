#include "color.h"
#include <stdlib.h>
#include <math.h>

static double euclidean_distance_squared(Color p1, Color p2)
{
	return pow(p2.L - p1.L, 2) + pow(p2.a - p1.a, 2) + pow(p2.b - p1.b, 2);
}

double euclidean_distance(Color p1, Color p2)
{
  return sqrt(euclidean_distance_squared(p1, p2));
}

Color color_average(Color* colors, int length)
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

	Color result;
	result.L = L / (double)length;
	result.a = a / (double)length;
	result.b = b / (double)length;

	return result;
}

int i_compare_L(Color c1, Color c2)
{
	if(c1.L < c2.L) return -1;
	else if(c1.L > c2.L) return 1;
	else return 0;
}

int i_compare_a(Color c1, Color c2)
{
	if(c1.a < c2.a) return -1;
	else if(c1.a > c2.a) return 1;
	else return 0;
}

int i_compare_b(Color c1, Color c2)
{
	if(c1.b < c2.b) return -1;
	else if(c1.b > c2.b) return 1;
	else return 0;
}

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

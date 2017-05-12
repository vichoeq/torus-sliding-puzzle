#include "color.h"
#include <stdlib.h>
#include <math.h>

/** Computes the square of the euclidian distance between two colors */
static double euclidean_distance_squared(Color p1, Color p2)
{
	return pow(p2.L - p1.L, 2) + pow(p2.a - p1.a, 2) + pow(p2.b - p1.b, 2);
}

/** Computes the euclidian distance between two colors */
double euclidean_distance(Color p1, Color p2)
{
  return sqrt(euclidean_distance_squared(p1, p2));
}

/** Computes the arithmetic mean of an array of colors */
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

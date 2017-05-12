#pragma once

struct color_lab
{
	/** Lightness channel */
  double L;
	/** Magenta / Green channel */
  double a;
	/** Blue / Yellow channel */
  double b;
};
/** Represents a color in L*a*b color space */
typedef struct color_lab Color;

/** Computes the arithmetic mean of an array of colors */
Color color_average(Color* colors, int length);
/** Computes the euclidian distance between two colors */
double euclidean_distance(Color p1, Color p2);

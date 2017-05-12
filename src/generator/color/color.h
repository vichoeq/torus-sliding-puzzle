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
/** Representa un color en espacio Lab */
typedef struct color_lab Color;

typedef enum axis
{
	L,
	a,
	b
} Axis;


Color color_average(Color* colors, int length);

int i_compare_L(Color c1, Color c2);

int i_compare_a(Color c1, Color c2);

int i_compare_b(Color c1, Color c2);
int compare(Color c1, Color c2, Axis ax);
double euclidean_distance(Color p1, Color p2);

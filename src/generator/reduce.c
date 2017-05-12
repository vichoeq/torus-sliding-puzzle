#include "reduce.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kdtree/kdtree.h"
#include <time.h>

/** Reduces the color amount of the image so it has at most 2^k */
void reduce(Image* image, int k)
{
	int length = image -> width * image -> height;

	Color* array = calloc(length, sizeof(Color));
	length = 0;

	/* Convert the image to a color array  */
	for(int row = 0; row < image -> height; row++)
	{
		for(int col = 0; col < image -> width; col++)
		{
			array[length++] = image -> pixels[row][col];
		}
	}

	/* Build the palette using median cut, also structure within a kd-tree */
	KDTree* kd = kd_create(array, length, k);

	/* Replace each color on the image with the closest in the palette */
	for(int row = 0; row < image -> height; row++)
	{
		for(int col = 0; col < image -> width; col++)
		{
			image -> pixels[row][col] = kd_find_closest(kd, image -> pixels[row][col]);
		}
	}

	kd_destroy(kd);
}

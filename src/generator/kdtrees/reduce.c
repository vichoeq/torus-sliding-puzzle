#include "../imagelib/imagelib.h"
#include "reduce.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "geometry.h"
#include "kdtree.h"
#include <time.h>

void reduce(Image_lab *image, int k)
{
	int length = image -> width * image -> height;
	printf("Image dimensions: %d x %d -> %d pixels\n", image -> width, image -> height, length);
	printf("K = %d, pallete will have %d colors\n", k, (int)pow(2,k));

	Color_lab* array = malloc(sizeof(Color_lab) * length);
	for(int i = 0; i < length; i++)
	{
		array[i] = image -> pixels[i];
	}

	double error = 0;

	printf("Creating pallete tree\n");
	clock_t start = clock();
	KDTree* kd = kd_create(array, length, k);

	printf("Pallete tree computed in %lf seconds\n\t Proceeding to reduce\n",  (double)(clock() - start) / CLOCKS_PER_SEC);

	start = clock();
  /*
   * Encontramos el color más cercano y reemplazamos el pixel.
   */
	for(int i = 0; i < length; i++)
	{
      // Reemplazamos el contenido del pixel/color original.

			Color_lab closest = kd_find_closest(kd, image -> pixels[i]);
			// Color_lab closest = kd_find_closest_greedy(kd, image -> pixels[i]);

			error+=euclidean_distance(closest, image -> pixels[i]);

			image -> pixels[i] = closest;
  }

	double reducetime = (double)(clock() - start) / CLOCKS_PER_SEC;

	printf("Image reduced in %lf seconds\n",  reducetime);
	printf("An average of %lf per pixel\n", reducetime / length);
	printf("E = %lf, average %lf\n", error, error/length);
	kd_destroy(kd);

}

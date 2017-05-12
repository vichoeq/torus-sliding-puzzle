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

	/* Convertir la imagen a un arreglo de colores */
	Color* array = malloc(sizeof(Color) * length);
	for(int i = 0; i < length; i++)
	{
		array[i] = image -> pixels[i];
	}

	KDTree* kd = kd_create(array, length, k);

	/* Encontramos el color más cercano y reemplazamos el pixel */
	for(int i = 0; i < length; i++)
	{
		image -> pixels[i] = kd_find_closest(kd, image -> pixels[i]);
  }

	kd_destroy(kd);
}

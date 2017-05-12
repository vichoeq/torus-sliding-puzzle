#include "imagelib/imagelib.h"
#include "kdtrees/reduce.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "images/imagelib.h"

int main(int argc, char *argv[])
{
  if (argc < 4)
	{
    printf("Usage: %s <source> <target> <k>\nwhere\n", argv[0]);
		printf("\t<source> is the source image\n");
		printf("\t<target> is the target image\n");
		printf("\t2^<k> is the maximum number of colors\n");
    return 1;
  }

	/* Cargar la imagen */
	Image_lab *image = load_image_lab(argv[1]);

	/* Procesar la imagen */
  reduce(image, atoi(argv[3]));

  /* Escribir la imagen resultante */
  write_image_lab(argv[2], image);

	/* Liberar los recursos */
  free_image_lab(image);

  return 0;
}

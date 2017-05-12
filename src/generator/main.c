#include <stdio.h>
#include <stdlib.h>
#include "reduce.h"
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
	Image* image = img_png_read_from_file(argv[1]);

	/* Procesar la imagen */
  reduce(image, atoi(argv[3]));

  /* Escribir la imagen resultante */
	img_png_write_to_file(image, argv[2]);

	/* Liberar los recursos */
	img_png_destroy(image);

  return 0;
}

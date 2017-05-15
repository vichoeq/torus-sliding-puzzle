#include <stdio.h>
#include <stdlib.h>
#include "imagelib/imagelib.h"
#include "generate.h"
#include "../random/pcg_basic.h"

int main(int argc, char *argv[])
{
  if (argc < 4)
	{
    printf("Usage: %s <source> <seed> <entropy> \nwhere\n", argv[0]);
		printf("\t<source> is the source image\n");
    return 1;
  }

	/* Semilla aleatoria */
	pcg32_srandom(0, atoi(argv[2]));

	/* Cargar la imagen */
	Image* image = img_png_read_from_file(argv[1]);

	generate_puzzle(image, atoi(argv[3]));

	/* Liberar los recursos */
	img_png_destroy(image);

  return 0;
}

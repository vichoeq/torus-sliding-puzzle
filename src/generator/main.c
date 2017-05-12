#include <stdio.h>
#include <stdlib.h>
#include "imagelib/imagelib.h"
#include "generate.h"

int main(int argc, char *argv[])
{
  if (argc < 2)
	{
    printf("Usage: %s <source>\nwhere\n", argv[0]);
		printf("\t<source> is the source image\n");
    return 1;
  }

	/* Cargar la imagen */
	Image* image = img_png_read_from_file(argv[1]);

	generate_puzzle(image);

	/* Liberar los recursos */
	img_png_destroy(image);

  return 0;
}

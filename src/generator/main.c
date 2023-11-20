#include <stdio.h>
#include <stdlib.h>
#include "imagelib/imagelib.h"
#include "generate.h"
#include "../random/pcg_basic.h"
#include <stdbool.h>
#include <string.h>

static void check_parameters(int argc, char** argv)
{
	if(argc == 5 && !strcmp(argv[3],"-i")) return;

	if (argc < 5)
	{
		printf("Usage:\n\n");
		printf("\t %s <seed> <entropy> <height> <width>\n", argv[0]);
		printf("\t %s <seed> <entropy> -i <image>\n\n", argv[0]);
		printf("<seed> is the random seed\n");
		printf("<entropy> how many random moves are applied to the puzzle\n");
		printf("<height> amount of rows of the puzzle, less than 64\n");
		printf("<width> amount of columns of the puzzle, less than 64\n");
		printf("<image> is the path to a .png image, which must follow width and height restrictions and have at most 8 different colors\n");
    exit(1);
  }
	int entropy = atoi(argv[2]);
	if(entropy < 0)
	{
		printf("Invalid value for entropy: %d\n", entropy);
		exit(2);
	}
	int height = atoi(argv[3]);
	if(!height || height >= 64)
	{
		printf("Invalid value for height: %d\n", height);
		exit(3);
	}
	int width = atoi(argv[4]);
	if(!width || width >= 64)
	{
		printf("Invalid value for width: %d\n", width);
		exit(4);
	}
}

int main(int argc, char *argv[])
{
	/* Revisa parámetros */
  check_parameters(argc, argv);

	/* Semilla aleatoria */
	pcg32_srandom(0, atoi(argv[1]));

	/* Modo imagen */
	if(!strcmp(argv[3],"-i"))
	{
		/* Cargar la imagen */
		Image* image = img_png_read_from_file(argv[4]);

		generate_from_image(image, atoi(argv[2]));

		/* Liberar los recursos */
		img_png_destroy(image);
	}
	/* Modo aleatorio */
	else
	{
		generate_from_scratch(atoi(argv[3]), atoi(argv[4]), atoi(argv[2]));
	}


  return 0;
}

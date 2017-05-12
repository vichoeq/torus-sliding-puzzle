#include "imagelib/imagelib.h"
#include "kdtrees/reduce.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Usage: %s <source> <target> <k>\nwhere\n", argv[0]);
		printf("\t<source> is the source image\n");
		printf("\t<target> is the target image\n");
		printf("\t2^<k> is the maximum number of colors\n");
    return 1;
  }


	printf("Loading image %s\n", argv[1]);
	clock_t start = clock();

	Image_lab *image = load_image_lab(argv[1]);


	printf("Image loaded in %lf seconds\n", (double)(clock() - start) / CLOCKS_PER_SEC);

  // OJO: a reduce le pasamos un puntero a la imagen y el número de colores.
	start = clock();
  reduce(image, atoi(argv[3]));
	printf("Student took %lf seconds\n", (double)(clock() - start) / CLOCKS_PER_SEC);

  /*
   * Escribimos la imagen, que fue procesada por reduce().
   */
	printf("Writing image\n");
	start = clock();
  write_image_lab(argv[2], image);
	printf("Image written in %lf seconds\n", (double)(clock() - start) / CLOCKS_PER_SEC);

	printf("Freeing image\n");
	start = clock();
  free_image_lab(image);
	printf("Image freed in %lf seconds\n", (double)(clock() - start) / CLOCKS_PER_SEC);


  return 0;
}

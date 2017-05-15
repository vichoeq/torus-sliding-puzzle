#include <stdlib.h>
#include <stdio.h>
#include "generate.h"
#include "../puzzle/puzzle.h"
#include "../puzzle/operation.h"
#include "palette.h"
#include "../random/pcg_basic.h"

void shuffle_puzzle(Puzzle* puz, int entropy)
{
	Operation* ops = operation_generate_all(puz);

	int size = 2 * puz -> width + 2 * puz -> height;

	for(int i = 0; i < entropy; i++)
	{

		Operation op = ops[pcg32_boundedrand(size)];
		operation_execute(puz, op);

		operation_print(op, stderr);
	}

	free(ops);
}

/** Genera un puzzle nuevo a partir de una imagen */
void generate_puzzle(Image* img, int entropy)
{
	Color* pal = palette_extract(img);

	palette_print(pal, stdout);

	Puzzle* puz = malloc(sizeof(Puzzle));
	puz -> width = img -> width;
	puz -> height = img -> height;
	puz -> matrix = calloc(puz -> height, sizeof(uint8_t*));
	for(uint8_t row = 0; row < puz -> height; row++)
	{
		puz -> matrix[row] = calloc(puz -> width, sizeof(uint8_t));
		for(uint8_t col = 0; col < puz -> width; col++)
		{
			puz -> matrix[row][col] = palette_lookup(pal, img -> pixels[row][col]);
		}
	}

	shuffle_puzzle(puz, entropy);

	puzzle_print(puz, stdout);
	puzzle_destroy(puz);
	free(pal);
}

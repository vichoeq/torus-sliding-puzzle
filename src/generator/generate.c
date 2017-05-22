#include <stdlib.h>
#include <stdio.h>
#include "generate.h"
#include "../puzzle/puzzle.h"
#include "../puzzle/operation.h"
#include "palette.h"
#include "../random/pcg_basic.h"

/** Desordena el puzzle. Mientras más alta la entropía, más desordenado */
void puzzle_shuffle(Puzzle* puz, uint32_t entropy)
{
	Operation* ops = operation_generate_all(puz);

	int size = 2 * puz -> width + 2 * puz -> height;

	for(int i = 0; i < entropy; i++)
	{
		Operation op = ops[pcg32_boundedrand(size)];
		operation_execute(puz, op);
	}

	free(ops);
}

/** Genera un puzzle nuevo a partir de una imagen */
void generate_from_image(Image* img, uint32_t entropy)
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
	free(pal);
	/* Imprime el estado final */
	puzzle_print(puz, stdout);
	/* Lo revuelve */
	puzzle_shuffle(puz, entropy);
	/* Imprime el estado inicial */
	puzzle_print(puz, stdout);
	/* Libera la memoria */
	puzzle_destroy(puz);
}

void generate_from_scratch(uint8_t height, uint8_t width, uint32_t entropy)
{
	Color* pal = palette_default();
	palette_print(pal, stdout);
	free(pal);

	Puzzle* puz = malloc(sizeof(Puzzle));

	puz -> height = height;
	puz -> width = width;

	puz -> matrix = calloc(puz -> height, sizeof(uint8_t*));
	for(uint8_t row = 0; row < puz -> height; row++)
	{
		puz -> matrix[row] = calloc(puz -> width, sizeof(uint8_t));
		for(uint8_t col = 0; col < puz -> width; col++)
		{
			puz -> matrix[row][col] = (row) % 8;
		}
	}
	/* Imprime el estado final */
	puzzle_print(puz, stdout);
	/* Lo revuelve */
	puzzle_shuffle(puz, entropy);
	/* Imprime el estado inicial */
	puzzle_print(puz, stdout);
	/* Libera la memoria */
	puzzle_destroy(puz);
}

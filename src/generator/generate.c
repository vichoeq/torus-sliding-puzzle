#include "generate.h"
#include "../puzzle/puzzle.h"
#include "palette.h"
#include <stdlib.h>
#include <stdio.h>

void puzzle_print(Puzzle* puz, void* stream)
{
	fprintf(stream, "%hhu\n", puz -> height);
	fprintf(stream, "%hhu\n", puz -> width);
	for(uint8_t row = 0; row < puz -> height; row++)
	{
		for(uint8_t col = 0; col < puz -> width; col++)
		{
			fprintf(stream, "%hhu ", puz -> matrix[row][col]);
		}
		fprintf(stream, "\n");
	}
}

void generate_puzzle(Image* img)
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
	puzzle_print(puz, stdout);
	puzzle_destroy(puz);
	free(pal);
}

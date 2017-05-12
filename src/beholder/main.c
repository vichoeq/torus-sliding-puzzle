#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../puzzle/puzzle.h"
#include "window.h"
#include "drawing.h"
#include "color.h"
#include <math.h>

static double compute_cell_size(Puzzle* puz)
{
	/* Tamaño por defecto del canvas */
	double cell_size     = CELL_SIZE;
	double canvas_height = cell_size * (puz -> height + 1);
	double canvas_width  = cell_size * (puz -> width + 1);

	/* Restricción de tamaño */
	double scale = MAX_DIMENSION / fmax(canvas_width, canvas_height);

	/* Escalamiento */
	return cell_size *= scale;
}

/** Lee el estado inicial del puzzle a partir de la consola */
static Puzzle* initial_state()
{
	Puzzle* puz = malloc(sizeof(Puzzle));

	puz -> height = 3;
	puz -> width = 2;

	// fscanf(stdin, "%hhu", &puz -> height);
	// fscanf(stdin, "%hhu", &puz -> width);

	puz -> matrix = calloc(puz -> height, sizeof(uint8_t*));
	for(uint8_t row = 0; row < puz -> height; row++)
	{
		puz -> matrix[row] = calloc(puz -> width, sizeof(uint8_t));
		for(uint8_t col = 0; col < puz -> width; col++)
		{
			// puz -> matrix[row][col] = 0;
			puz -> matrix[row][col] = (row+col) % 8;
			// fscanf(stdin, "%hhu", &puz -> matrix[row][col]);
		}
	}
	return puz;
}

static bool check_parameters(int argc, char** argv)
{
	return argc == 1;
}

int main(int argc, char** argv)
{
	/* Se revisa que los parametros entregados sean correctos */
  if(!check_parameters(argc, argv)) return 1;

	srand(time(NULL));

	Puzzle* puz = initial_state();

	Content cont =
	{
		.color_table = color_table(),
		.puz = puz,
		.mode = ALL,
		.cell_size = compute_cell_size(puz),
		.offset = 0,
		.index = 0
	};

	drawing_init(cont.color_table, cont.cell_size);

	window_create(&cont);

	drawing_free();
	puzzle_destroy(cont.puz);
	free(cont.color_table);

	return 0;
}

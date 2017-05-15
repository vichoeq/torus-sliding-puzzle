#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../puzzle/puzzle.h"
#include "../puzzle/operation.h"
#include "window.h"
#include "drawing/drawing.h"
#include "drawing/color.h"
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

static bool check_parameters(int argc, char** argv)
{
	return argc == 1;
}

int main(int argc, char** argv)
{
	/* Se revisa que los parametros entregados sean correctos */
  if(!check_parameters(argc, argv)) return 1;

	srand(time(NULL));


	Color* palette = color_table();

	Puzzle* puz = puzzle_from_stream(stdin);

	Content cont =
	{
		.color_table = palette,
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

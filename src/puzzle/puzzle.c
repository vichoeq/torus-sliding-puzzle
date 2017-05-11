#include "puzzle.h"
#include <stdlib.h>

/** Lee un puzzle a partir de un archivo */
Puzzle* puzzle_from_file (char* path)
{
	abort();
}

/** Libera los recursos asociados a un  puzzle */
void puzzle_destroy(Puzzle* puzzle)
{
	for(uint8_t row = 0; row < puzzle -> height; row++)
	{
		free(puzzle -> matrix[row]);
	}
	free(puzzle -> matrix);
	free(puzzle);
}

/** Desliza la fila especificada hacia la izquierda */
void puzzle_shift_left(Puzzle* puz, uint8_t row)
{
	uint8_t aux = puz -> matrix[row][0];
	for(uint8_t col = 0; col < puz -> width - 1; col++)
	{
		puz -> matrix[row][col] = puz -> matrix[row][col + 1];
	}
	puz -> matrix[row][puz -> width - 1] = aux;
}

void puzzle_shift_right(Puzzle* puz, uint8_t row)
{
	uint8_t aux = puz -> matrix[row][puz -> width - 1];
	for(uint8_t col = puz -> width - 1; col > 0; col--)
	{
		puz -> matrix[row][col] = puz -> matrix[row][col - 1];
	}
	puz -> matrix[row][0] = aux;
}

void puzzle_shift_up(Puzzle* puz, uint8_t col)
{
	uint8_t aux = puz -> matrix[0][col];
	for(uint8_t row = 0; row < puz -> height - 1; row++)
	{
		puz -> matrix[row][col] = puz -> matrix[row + 1][col];
	}
	puz -> matrix[puz -> height - 1][col] = aux;
}

void puzzle_shift_down(Puzzle* puz, uint8_t col)
{
	uint8_t aux = puz -> matrix[puz -> height - 1][col];
	for(uint8_t row = puz -> height - 1; row > 0; row--)
	{
		puz -> matrix[row][col] = puz -> matrix[row - 1][col];
	}
	puz -> matrix[0][col] = aux;
}

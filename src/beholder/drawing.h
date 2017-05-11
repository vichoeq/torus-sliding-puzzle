#pragma once

#include <cairo.h>
#include <stdbool.h>
#include "color.h"
#include "../puzzle/puzzle.h"

#define CELL_SIZE 64.0
#define MAX_DIMENSION (512.0 + 128.0)

/** Indica el modo de dibujo */
enum draw_mode
{
	/** Dibuja una fila */
	ROW,
	/** Dibuja una columna */
	COL,
	/** Dibuja el tablero entero */
	ALL
};

/** Indica el modo de dibujo */
typedef enum draw_mode Mode;


/** Contiene la información gráfica del puzzle */
struct content
{
	/** El puzzle del canvas */
	Puzzle* puz;
	/** Tamaño de una celda */
	double const cell_size;
	/** La tabla con los distintos colores */
	Color* color_table;
	/** Qué es lo que hay que dibujar */
	Mode mode;
	/** La variación en posición de una de las filas / columnas */
	double offset;
	/** Cual fila / columna está variando */
	uint8_t index;
};
/** Contiene la información de las dimensiones del canvas */
typedef struct content Content;

void drawing_init(Color* color_table, double const cell_size);

void drawing_free();
/** Dibuja sobre el canvas dado */
bool canvas_draw(cairo_t* cr, Content* cont);

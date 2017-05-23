#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cairo-pdf.h>
#include "drawing.h"


/** La razón entre ambos cuadrados de una celda */
#define RATIO 0.5
/** Color de fondo */
#define BGC (Color){.R = 27.0 /255.0, .G = 27.0 /255.0,.B = 47.0 /255.0}
/** Color de las flechas */
#define WTE (Color){.R = 200.0/255.0, .G = 200.0/255.0,.B = 200.0/255.0}
/** Color de las flechas apretadas */
#define PRS (Color){.R = 0.0/255.0, .G = 0.0/255.0,.B = 255.0/255.0}

/* ------------------------- Variables Globales --------------------------- */

/** Cada uno de los bloques dibujado por separado */
cairo_surface_t* block_sprites[8];

/* ------------------------------ Útiles ---------------------------------- */

/** Multiplica un color por un escalar, para aclararlo o oscurecerlo */
static Color color_scale(Color color, double k)
{
  return (Color){.R = color.R * k,.G = color.G * k, .B = color.B * k};
}

/** Setea el color RGB de cairo */
static void color_dip(cairo_t* cr, Color color)
{
  cairo_set_source_rgb(cr,color.R,color.G, color.B);
}

static double gap_width(double const cell_size)
{
	double const ratio = 0.15 - cell_size/(MAX_DIMENSION*4);

	return ratio * cell_size + 2.2;
}

/* ----------------------------- Flechas ---------------------------------- */

/** Dibuja el cuarto superior de un cuadrado */
static void draw_top_arrow(cairo_t* cr, Color color, double x, double y, double size)
{
  cairo_move_to(cr, x - size/2, y - size/2);
  cairo_rel_line_to(cr, size, 0);
  cairo_line_to(cr,x,y);
  cairo_close_path(cr);
  color_dip(cr, color);
  cairo_fill(cr);
}

/** Dibuja el cuarto izquierdo de un cuadrado */
static void draw_left_arrow(cairo_t* cr, Color color, double x, double y, double size)
{
  cairo_move_to(cr, x - size/2, y - size/2);
  cairo_rel_line_to(cr, 0, size);
  cairo_line_to(cr,x,y);
  cairo_close_path(cr);
  color_dip(cr, color);
  cairo_fill(cr);
}

/** Dibuja el cuarto derecho de un cuadrado */
static void draw_right_arrow(cairo_t* cr, Color color, double x, double y, double size)
{
  cairo_move_to(cr, x + size/2, y - size/2);
  cairo_rel_line_to(cr, 0, size);
  cairo_line_to(cr,x,y);
  cairo_close_path(cr);
  color_dip(cr, color);
  cairo_fill(cr);
}

/** Dibuja el cuarto inferior de un cuadrado */
static void draw_bottom_arrow(cairo_t* cr, Color color, double x, double y, double size)
{
  cairo_move_to(cr, x - size/2, y + size/2);
  cairo_rel_line_to(cr, size, 0);
  cairo_line_to(cr,x,y);
  cairo_close_path(cr);
  color_dip(cr, color);
  cairo_fill(cr);
}

/* ----------------------------- Bloques ---------------------------------- */


/** Dibuja un bloque centrado en x e y, del tamaño especificado */
static void draw_block(cairo_t* cr, Color color, double const x, double const y, double const size)
{
  /* Las dimensiones del cuadrado mayor, menos un poco para dejar espacio */
  double outer = size - gap_width(size);
  /* Establecemos el grosor de linea */
  cairo_set_line_width(cr, size/10);

  /* Rellena el cuadrado entero primero */
  cairo_rectangle(cr, x - outer / 2, y - outer/2, outer, outer);
  color_dip(cr,color);
  cairo_fill(cr);

  /* Top fragment */
  draw_top_arrow(cr, color_scale(color, 1.5), x, y, outer);

  /* Left Fragment */
  draw_left_arrow(cr, color_scale(color, 1.25), x, y, outer);

  /* Right Fragment */
  draw_right_arrow(cr, color_scale(color, 0.75), x, y, outer);

  /* Bottom fragment */
  draw_bottom_arrow(cr, color_scale(color, 0.5), x, y, outer);

  /* Dibujamos el cuadrado interno */
  double inner = RATIO * outer;
  cairo_rectangle(cr, x - inner/2, y - inner/2, inner, inner);
  color_dip(cr, color_scale(color, 1));
  cairo_fill(cr);
}

static void draw_frame(cairo_t* restrict cr, Content* restrict cont)
{
	double sx, sy, w, h;

	/* Dibuja una sombra por el lado superior izquierdo */

	sx = cont -> cell_size / 2;
	sy = cont -> cell_size / 2;
	w = cont -> puz -> width * cont -> cell_size;
	h = cont -> puz -> height * cont -> cell_size;

	cairo_move_to(cr, sx, sy + h + gap_width(cont -> cell_size));
	cairo_line_to(cr, sx, sy);
	cairo_line_to(cr, sx + w + gap_width(cont -> cell_size), sy);

	cairo_set_operator(cr, CAIRO_OPERATOR_MULTIPLY);

	cairo_set_source_rgba(cr, 13.5 /255.0, 13.5 /255.0, 23.5 /255.0, 0.5);
	cairo_set_line_width(cr, cont -> cell_size * 0.4);
	cairo_stroke(cr);

	cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

	/* Dibuja el borde blanco */
	sx -= gap_width(cont -> cell_size);
	sy -= gap_width(cont -> cell_size);
	w  += 2 * gap_width(cont -> cell_size);
	h  += 2 * gap_width(cont -> cell_size);

	cairo_rectangle(cr, sx, sy, w, h);

	color_dip(cr, WTE);
	cairo_set_line_width(cr, gap_width(cont -> cell_size) * 1.05);
	cairo_stroke_preserve(cr);

	/* Pinta el marco exterior */

	cairo_new_sub_path(cr);

	sx = 0;
	sy = 0;
	w = (cont -> puz -> width + 1) * cont -> cell_size;
	h = (cont -> puz -> height + 1) * cont -> cell_size;

	cairo_rectangle(cr, sx, sy, w, h);
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
	color_dip(cr, color_scale(WTE,0.4));

	cairo_fill(cr);

	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
}

bool drawing_draw(cairo_t* restrict cr, Content* restrict cont)
{
	pthread_mutex_lock(&drawing_mutex);

	/* Color de fondo */
	color_dip(cr, BGC);
	cairo_paint(cr);

	uint8_t width = cont -> puz -> width;
	uint8_t height = cont -> puz -> height;

	Mode mode = cont -> mode;

	int16_t row_start = mode == ROW ? cont -> index : -1;
	int16_t row_end = mode == ROW ? cont -> index : height;

	int16_t col_start = mode == COL ? cont -> index : -1;
	int16_t col_end = mode == COL ? cont -> index : width;

	double x_off = mode == ROW ? cont -> offset : 0;
	double y_off = mode == COL ? cont -> offset : 0;

	/* Dibuja los bloques */
	for(int16_t row = row_start; row <= row_end; row++)
  {
    for(int16_t col = col_start; col <= col_end; col++)
    {
			int r = (height + row) % height;
			int c = (width + col) % width;
			int index = cont -> puz -> matrix[r][c];

			double cx = (col + 0.5) * cont -> cell_size + x_off;
			double cy = (row + 0.5) * cont -> cell_size + y_off;

			cairo_set_source_surface(cr, block_sprites[index], cx, cy);
			cairo_rectangle(cr, cx, cy, cont -> cell_size,cont -> cell_size);
			cairo_fill(cr);
    }
  }

	/* Tapa los bloques exteriores */
	draw_frame(cr, cont);

	pthread_mutex_unlock(&drawing_mutex);

	return true;
}

void drawing_free()
{
	for(int i = 0; i < 8; i++)
	{
		cairo_surface_destroy(block_sprites[i]);
	}

	pthread_mutex_destroy(&drawing_mutex);
}

/** Genera de antemano las imagenes para los distintos bloques */
void drawing_init(Color* color_table, double const cell_size)
{
	for(int i = 0; i < 8; i++)
	{
		block_sprites[i] = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, ceil(cell_size),  ceil(cell_size));
		cairo_t* cr = cairo_create(block_sprites[i]);

		cairo_set_source_rgba(cr, 0, 0, 0, 0);
		cairo_paint(cr);

		draw_block(cr, color_table[i], cell_size/2, cell_size/2, cell_size);

		cairo_destroy(cr);
	}

	pthread_mutex_init(&drawing_mutex, NULL);
}

static void snapshot_draw(cairo_t* restrict cr, Content* restrict cont)
{
	/* Color de fondo */
	// color_dip(cr, BGC);
	cairo_set_source_rgba(cr, 1, 1, 1, 0);
	cairo_paint(cr);

	uint8_t width = cont -> puz -> width;
	uint8_t height = cont -> puz -> height;

	/* Dibuja los bloques */
	for(int16_t row = 0; row < height; row++)
  {
    for(int16_t col = 0; col < width; col++)
    {
			int r = (height + row) % height;
			int c = (width + col) % width;
			int index = cont -> puz -> matrix[r][c];

			double cell_size = cont -> cell_size;

			double cx = (col + 0.5) * cell_size;
			double cy = (row + 0.5) * cell_size;

			draw_block(cr, cont -> color_table[index], cx, cy, cell_size);
    }
  }

	/* Tapa los bloques exteriores */
	// draw_frame(cr, cont);
}

/** Geenera una imagen en pdf para un estado en particular */
void drawing_snapshot(Content* cont, char* filename)
{
	const double cell_size = 3 * CELL_SIZE;

	double width = cell_size * (cont -> puz -> width);
	double height = cell_size * (cont -> puz -> height);

	/* Imprimimos las imagenes del tablero */
	cairo_surface_t* surface;
	cairo_t *cr;

	surface = cairo_pdf_surface_create (filename, width, height);
	cr = cairo_create(surface);

	/* Reseteamos los parámetros para generar correctamente la imagen */
	Content aux = (Content)
	{
		.cell_size = cell_size,
		.puz = cont -> puz,
		.color_table = cont -> color_table,
		.mode = ALL
	};

	/* Dibuja el estado actual */
	snapshot_draw(cr, &aux);

	cairo_surface_destroy(surface);
	cairo_destroy(cr);
}

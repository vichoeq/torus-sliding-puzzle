#pragma once

#include "../color/color.h"

struct image
{
	/** Matriz de colores que corresponde a la imagen */
	Color** pixels;
	/** Ancho de la imagen */
	int width;
	/** Alto de la imagen */
	int height;
};
/** Representa una imagen RGB como una matriz de colores */
typedef struct image Image;

/** Lee un archivo .png y entrega la imagen */
Image* img_png_read_from_file (char* filename);
/** Escribe el contenido de una imagen como archivo .png */
void   img_png_write_to_file  (Image* img, char* filename);

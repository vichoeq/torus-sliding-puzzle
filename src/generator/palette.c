#include "palette.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>



/** Entrega la paleta de colores por defecto */
Color*  palette_default ()
{
	Color* table = calloc(8, sizeof(Color));

	table[0] = (Color){.R = 0, .G = 0, .B = 0};
	table[1] = (Color){.R = 0, .G = 0, .B = 255};
	table[2] = (Color){.R = 0, .G = 255, .B = 0};
	table[3] = (Color){.R = 0, .G = 255, .B = 255};
	table[4] = (Color){.R = 255, .G = 0, .B = 0};
	table[5] = (Color){.R = 255, .G = 0, .B = 255};
	table[6] = (Color){.R = 255, .G = 255, .B = 0};
	table[7] = (Color){.R = 255, .G = 255, .B = 255};

	return table;
}

/** Indica si dos colores son o no iguales */
static bool color_equals(Color c1, Color c2)
{
	return c1.R == c2.R && c1.G == c2.G && c1.B == c2.B;
}

/** Obtiene el indice en la paleta para un color dado */
uint8_t palette_lookup(Color* palette, Color color)
{
	for(int i = 0; i < 8; i++)
	{
		if(color_equals(palette[i], color))
		{
			return i;
		}
	}
	abort();
}

/** Imprime la paleta de colores al stream dado */
void palette_print(Color* palette, void* stream)
{
	for(int i = 0; i < 8; i++)
	{
		Color c = palette[i];
		fprintf(stream, "%hhu %hhu %hhu\n", c.R, c.G, c.B);
	}
}

/** Indica que el color es distinto a todos los colores de la paleta */
static bool different_from_all(Color* palette, Color c, uint8_t count)
{
	//TODO hay formas más elegantes de hacer esto
	for(int i = 0; i < count; i++)
	{
		if(color_equals(c, palette[i]))
		{
			return false;
		}
	}
	return true;
}

/** Obtiene la paleta de colores a partir de una imagen */
Color* palette_extract(Image* img)
{
	Color* palette = calloc(8, sizeof(Color));

	uint8_t count = 0;

	for(int row = 0; row < img -> height; row++)
	{
		for(int col = 0; col < img -> width; col++)
		{
			Color c = img -> pixels[row][col];

			if(different_from_all(palette, c, count))
			{
				palette[count++] = c;
			}
		}
	}

	assert(count <= 8);

	return palette;
}

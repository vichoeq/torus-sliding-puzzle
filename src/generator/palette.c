#include "palette.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

bool color_equals(Color c1, Color c2)
{
	return c1.R == c2.R && c1.G == c2.G && c1.B == c2.B;
}

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

void palette_print(Color* palette, void* stream)
{
	for(int i = 0; i < 8; i++)
	{
		Color c = palette[i];
		fprintf(stream, "%hhu %hhu %hhu\n", c.R, c.G, c.B);
	}
}

bool different_from_all(Color* palette, Color c, uint8_t count)
{
	for(int i = 0; i < count; i++)
	{
		if(color_equals(c, palette[i]))
		{
			return false;
		}
	}
	return true;
}

/** Obtains the palette of an image */
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

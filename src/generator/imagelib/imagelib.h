#pragma once

#include <stdint.h>

struct color
{
	/** Red channel */
	uint8_t R;
	/** Green channel */
	uint8_t G;
	/** Blue channel */
	uint8_t B;
};
/** Representa un color en RGB */
typedef struct color Color;

struct image
{
	/** 2D Matrix of colors, the image itself */
	Color** pixels;
	/** Width of the image, size of a row */
	int width;
	/** Height of the image, size of a column */
	int height;
};
/** Represents a L*a*b image as a simple color matrix */
typedef struct image Image;

/** Reads a .png file and converts it to a color matrix */
Image* img_png_read_from_file (char* filename);
/** Writes a color matrix on to a .png file */
void   img_png_write_to_file  (Image* img, char* filename);
/** Frees the resources used by the image */
void   img_png_destroy        (Image* img);

/*
* Basado en código de ejemplo para libpng
* https://gist.github.com/niw/5963798
*/
#include "imagelib.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <math.h>

/** Lee un archivo .png y entrega la imagen */
Image* img_png_read_from_file (char* filename)
{
	Image* img = malloc(sizeof(Image));

  FILE *fp = fopen(filename, "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png) abort();

  png_infop info = png_create_info_struct(png);
  if(!info) abort();

  if(setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  png_read_info(png, info);

  int width        = png_get_image_width(png, info);
  int height       = png_get_image_height(png, info);
  uint8_t color_type = png_get_color_type(png, info);
  uint8_t bit_depth  = png_get_bit_depth(png, info);

  if(bit_depth == 16)
    png_set_strip_16(png);

  if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

	uint8_t** row_pointers = calloc(height, sizeof(uint8_t*));
  for(int row = 0; row < height; row++)
	{
		row_pointers[row] = calloc(width * 4, sizeof(uint8_t));
  }

  png_read_image(png, row_pointers);

	img -> pixels = calloc(height, sizeof(Color*));
	for(int row = 0; row < height; row++)
	{
		img -> pixels[row] = calloc(width, sizeof(Color));
		for(int col = 0; col < width; col++)
		{
			uint8_t* px = &(row_pointers[row][col * 4]);
			img -> pixels[row][col].R = px[0];
			img -> pixels[row][col].G = px[1];
			img -> pixels[row][col].B = px[2];
		}
		free(row_pointers[row]);
	}
	free(row_pointers);

  fclose(fp);

	png_destroy_read_struct(&png, &info, NULL);

	img -> width = width;
	img -> height = height;

	return img;
}

/** Escribe el contenido de una imagen como archivo .png */
void img_png_write_to_file(Image* img, char* filename)
{
	FILE *fp = fopen(filename, "wb");
  if(!fp) abort();

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) abort();

  png_infop info = png_create_info_struct(png);
  if (!info) abort();

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  // Output is 8bit depth, RGBA format.
  png_set_IHDR(
    png,
    info,
    img -> width, img -> height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_write_info(png, info);

	uint8_t** row_pointers = calloc(img -> height, sizeof(uint8_t*));
  for(int row = 0; row < img -> height; row++)
	{
		row_pointers[row] = calloc(img -> width * 4, sizeof(uint8_t));
		for(int col = 0; col < img -> width; col++)
		{
			uint8_t* px = &(row_pointers[row][col * 4]);
			px[0] = img -> pixels[row][col].R;
			px[1] = img -> pixels[row][col].G;
			px[2] = img -> pixels[row][col].B;
			px[3] = 255;
		}
  }

  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  for(int row = 0; row < img -> height; row++)
	{
    free(row_pointers[row]);
  }
  free(row_pointers);

  fclose(fp);

	if (png && info) png_destroy_write_struct(&png, &info);
}

/** Frees the resources used by the image */
void img_png_destroy(Image* img)
{
	for(int row = 0; row < img -> height; row++)
	{
		free(img -> pixels[row]);
	}
	free(img -> pixels);
	free(img);
}

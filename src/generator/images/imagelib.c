/*
* Based on example code for PNG
* https://gist.github.com/niw/5963798
*/
#include "imagelib.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <math.h>



/** Code by Vicho Dragicevic (https://github.com/vdrg) */
Color rgb_2_lab(uint8_t R, uint8_t G, uint8_t B)
{
  float var_R = R/255.0;
  float var_G = G/255.0;
  float var_B = B/255.0;


  if ( var_R > 0.04045 ) var_R = pow( (( var_R + 0.055 ) / 1.055 ), 2.4 );
  else                   var_R = var_R / 12.92;
  if ( var_G > 0.04045 ) var_G = pow( ( ( var_G + 0.055 ) / 1.055 ), 2.4);
  else                   var_G = var_G / 12.92;
  if ( var_B > 0.04045 ) var_B = pow( ( ( var_B + 0.055 ) / 1.055 ), 2.4);
  else                   var_B = var_B / 12.92;

  var_R = var_R * 100.;
  var_G = var_G * 100.;
  var_B = var_B * 100.;

  //Observer. = 2°, Illuminant = D65
  float X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
  float Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
  float Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;


  float var_X = X / 95.047 ;         //ref_X =  95.047   Observer= 2°, Illuminant= D65
  float var_Y = Y / 100.000;          //ref_Y = 100.000
  float var_Z = Z / 108.883;          //ref_Z = 108.883

  if ( var_X > 0.008856 ) var_X = pow(var_X , ( 1./3. ) );
  else                    var_X = ( 7.787 * var_X ) + ( 16. / 116. );
  if ( var_Y > 0.008856 ) var_Y = pow(var_Y , ( 1./3. ));
  else                    var_Y = ( 7.787 * var_Y ) + ( 16. / 116. );
  if ( var_Z > 0.008856 ) var_Z = pow(var_Z , ( 1./3. ));
  else                    var_Z = ( 7.787 * var_Z ) + ( 16. / 116. );

  float l_s = ( 116. * var_Y ) - 16.;
  float a_s = 500. * ( var_X - var_Y );
  float b_s = 200. * ( var_Y - var_Z );
  Color p = {l_s, a_s, b_s};
  return p;
}

/** Code by Vicho Dragicevic (https://github.com/vdrg) */
void lab_2_rgb(Color color, uint8_t* R, uint8_t* G, uint8_t *B)
{
	float l_s = color.L;
	float a_s = color.a;
	float b_s = color.b;

  float var_Y = ( l_s + 16. ) / 116.;
  float var_X = a_s / 500. + var_Y;
  float var_Z = var_Y - b_s / 200.;

  if ( pow(var_Y,3) > 0.008856 ) var_Y = pow(var_Y,3);
  else                      var_Y = ( var_Y - 16. / 116. ) / 7.787;
  if ( pow(var_X,3) > 0.008856 ) var_X = pow(var_X,3);
  else                      var_X = ( var_X - 16. / 116. ) / 7.787;
  if ( pow(var_Z,3) > 0.008856 ) var_Z = pow(var_Z,3);
  else                      var_Z = ( var_Z - 16. / 116. ) / 7.787;

  float X = 95.047 * var_X ;    //ref_X =  95.047     Observer= 2°, Illuminant= D65
  float Y = 100.000 * var_Y  ;   //ref_Y = 100.000
  float Z = 108.883 * var_Z ;    //ref_Z = 108.883


  var_X = X / 100. ;       //X from 0 to  95.047      (Observer = 2°, Illuminant = D65)
  var_Y = Y / 100. ;       //Y from 0 to 100.000
  var_Z = Z / 100. ;      //Z from 0 to 108.883

  float var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
  float var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415;
  float var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570;

  if ( var_R > 0.0031308 ) var_R = 1.055 * pow(var_R , ( 1 / 2.4 ))  - 0.055;
  else                     var_R = 12.92 * var_R;
  if ( var_G > 0.0031308 ) var_G = 1.055 * pow(var_G , ( 1 / 2.4 ) )  - 0.055;
  else                     var_G = 12.92 * var_G;
  if ( var_B > 0.0031308 ) var_B = 1.055 * pow( var_B , ( 1 / 2.4 ) ) - 0.055;
  else                     var_B = 12.92 * var_B;

  *R = (uint8_t) (var_R * 255.);
  *G = (uint8_t) (var_G * 255.);
  *B = (uint8_t) (var_B * 255.);
}

/** Reads a .png file and converts it to a color matrix */
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
			img -> pixels[row][col] = rgb_2_lab(px[0], px[1], px[2]);
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

/** Writes a color matrix on to a .png file */
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
			lab_2_rgb(img -> pixels[row][col], &px[0], &px[1], &px[2]);
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

#pragma once
#include <stdint.h>
#include "../color/color.h"

typedef struct image_lab {
  int width; // The width of the image in pixels
  int height; // The height of the image in pixels
  Color *pixels; // Pixel data
} Image_lab;

/*
 * Loads an image located in "path".
 */
Image_lab *load_image_lab(char const *pat);

/*
 * Receives a struct of type Image_lab, and writes it in "path".
 * "path" should end with .png or .bmp (i.e. "../hello.png").
 */
void write_image_lab(char const *path, Image_lab *image);

/*
 * Frees a pointer to an Image_lab struct.
 */
void free_image_lab(Image_lab *image);

/*
 * Receives a RGB colour and returns a transformed colour in the
 * lab colour space.
 */
Color rgb2lab( uint8_t R, uint8_t G, uint8_t B);

void lab2rgb( float l_s, float a_s, float b_s, uint8_t *R, uint8_t *G, uint8_t *B );

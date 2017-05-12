#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "imagelib.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

const char *get_filetype(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if(!dot || dot == filename) return "";
  return dot + 1;
}

Image_lab *load_image_lab(char const *path) {
  Image_lab *image = calloc(1, sizeof(Image_lab));
  int n; // number of channels given by the original format
  uint8_t *data = stbi_load(path, &image->width, &image->height, &n, 3);

  if (data == NULL) {
    puts("ERROR: Could not read image file");
    exit(EXIT_FAILURE);
  }

  int image_size = image->width * image->height;
  image->pixels = calloc(image_size, sizeof(Color_lab));
  for (int i = 0; i < image_size; i++) {
      // Create new pixel from the image's data
      uint8_t *pixel_data = &data[i * 3];

      Color_lab pixel = rgb2lab(pixel_data[0], pixel_data[1], pixel_data[2]);

      image->pixels[i] = pixel;
  }

  stbi_image_free(data);

  return image;
}

void write_image_lab(char const *path, Image_lab *image) {
  const char *filetype = get_filetype(path);
  if (strcmp(filetype, "png") != 0 && strcmp(filetype, "bmp") != 0) {
    puts("ERROR: output filename should end in .png or .bmp");
    exit(EXIT_FAILURE);
  }

  int size = image->width * image->height * 3;
  uint8_t *data = calloc(size, sizeof(uint8_t));

  unsigned long index = 0;
  for (int i = 0; i < image->width * image->height; i++) {
      uint8_t r, g, b;
      lab2rgb(image->pixels[i].L, image->pixels[i].a, image->pixels[i].b, &r, &g, &b);
      data[index++] = r;
      data[index++] = g;
      data[index++] = b;
  }

  int result = 0;
  if (strcmp(filetype, "png") == 0) {
    result = stbi_write_png(path, image->width, image->height, 3, data, 0);
  } else if (strcmp(filetype, "bmp") == 0) {
    result = stbi_write_bmp(path, image->width, image->height, 3, data);
  }

  if (result == 0) {
    puts("ERROR: could not write data");
    exit(EXIT_FAILURE);
  }

  free(data);
}

void free_image_lab(Image_lab *image) {
  free(image->pixels);
  free(image);
}

Color_lab rgb2lab(uint8_t R, uint8_t G, uint8_t B)
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
  Color_lab p = {l_s, a_s, b_s};
  return p;
}

void lab2rgb( float l_s, float a_s, float b_s, uint8_t *R, uint8_t *G, uint8_t *B )
{
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

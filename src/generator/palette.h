#pragma once

#include <stdint.h>
#include "imagelib/imagelib.h"

/** Entrega la paleta de colores por defecto */
Color*  palette_default ();
/** Obtiene la paleta de colores a partir de una imagen */
Color*  palette_extract (Image* img);
/** Imprime la paleta de colores al stream dado */
void    palette_print   (Color* palette, void* stream);
/** Obtiene el indice en la paleta para un color dado */
uint8_t palette_lookup  (Color* palette, Color color);

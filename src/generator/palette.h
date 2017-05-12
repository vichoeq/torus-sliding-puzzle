#pragma once

#include <stdint.h>
#include "imagelib/imagelib.h"

/** Obtains the underlying palette of the KD-Tree */
Color*  palette_extract (Image* img);
/** Prints the palette onto the given stream */
void    palette_print   (Color* palette, void* stream);
/** Finds the index of the given color */
uint8_t palette_lookup  (Color* palette, Color color);

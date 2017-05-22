#pragma once

#include "imagelib/imagelib.h"
#include "../puzzle/puzzle.h"

/** Genera un puzzle aleatorio desde 0 */
void generate_from_scratch(uint8_t height, uint8_t width, uint32_t entropy);
/** Genera un puzzle a partir de una imagen */
void generate_from_image(Image* img, uint32_t entropy);

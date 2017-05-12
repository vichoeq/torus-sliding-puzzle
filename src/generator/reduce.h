#pragma once

#include "images/imagelib.h"

/** Reduces the color amount of the image so it has at most 2^k */
void reduce(Image* image, int k);

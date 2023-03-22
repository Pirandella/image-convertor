#ifndef _IMAGE_PROCESSING_H_
#define _IMAGE_PROCESSING_H_

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "image.h"

void image_processing_invert(image_t *const image);

void image_processing_grayscale(image_t *const image, uint8_t bit_depth);

image_t *image_processing_rescale(image_t *image, size_t width, size_t height);

void image_processing_dithering(image_t *const image);

#endif /* _IMAGE_PROCESSING_H_ */
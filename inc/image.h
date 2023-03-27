#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	NOT_SET = 0,
	RGB = 3,
	RGBA = 4
} COLOR_TYPE;

typedef struct {
	size_t width;
	size_t height;
	COLOR_TYPE bytes_per_pixel;
	uint8_t **data;
} image_t;

image_t *image_init(void);

void image_allocate_data(image_t *image, size_t width, size_t height, COLOR_TYPE bytes_per_pixel);

void image_free(image_t *image);

#endif /* _IMAGE_H_ */
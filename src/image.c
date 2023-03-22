#include "image.h"

image_t *image_init(void)
{
	image_t *image = (image_t *)malloc(sizeof(image_t));
	image->width = 0;
	image->height = 0;
	image->capacity = image->width;
	image->bytes_per_pixel = NOT_SET;
	image->data = NULL;

	return image;
}

void image_allocate_data(image_t *image, size_t width, size_t height, COLOR_TYPE bytes_per_pixel)
{
	image->data = (uint8_t **)malloc(sizeof(uint8_t *) * height);
	for(size_t i = 0; i < height; i++)
		image->data[i] = (uint8_t *)calloc(1, width * bytes_per_pixel);
	
	image->width = width;
	image->height = height;
	image->capacity = image->width;
	image->bytes_per_pixel = bytes_per_pixel;
}

void image_free(image_t *image)
{
	if (image == NULL) return;
	if (image->data != NULL) { 
		for (size_t i = 0; i < image->capacity; i++)
			free(image->data[i]);
		free(image->data);
	}
	free(image);
}

#include "imageProcessing.h"

#define MIN(a, b) ((a < b) ? a : b)

void image_processing_invert(image_t *const image)
{
	for (size_t y = 0; y < image->height; y++) {
		for (size_t x = 0; x < image->width; x++) {
			uint8_t *pixel = &image->data[y][x * image->bytes_per_pixel];
			
			pixel[0] = ~pixel[0];
			pixel[1] = pixel[0];
			pixel[2] = pixel[0];
		}
	}
}

void image_processing_grayscale(image_t *const image, uint8_t bit_depth)
{
	uint8_t conversion_factor = 255 / (bit_depth - 1);
	
	for (uint32_t y = 0; y < image->height; y++) {
		for (uint32_t x = 0; x < image->width; x++) {
			uint8_t *pixel = &image->data[y][x * image->bytes_per_pixel];
			uint8_t avg_color = (uint8_t)(pixel[0] * 0.299f) + (uint8_t)(pixel[1] * 0.587f) + (uint8_t)(pixel[2] * 0.114f);
			uint8_t grayscale = (uint8_t)((avg_color / conversion_factor) + 0.5f) * conversion_factor;
			pixel[0] = grayscale;
			pixel[1] = grayscale;
			pixel[2] = grayscale;
		}
	} 
}

image_t *image_processing_rescale(image_t *image, size_t width, size_t height)
{
	image_t *_image = image_init();
	image_allocate_data(_image, width, height, image->bytes_per_pixel);

	/* Nearest Neighbor Image Scaling */
	for(size_t x = 0; x < width; x++) {
		for(size_t y = 0; y < height; y++) {
			size_t src_x = (size_t)roundf((float)x / (float)width * (float)image->width);
			size_t src_y = (size_t)roundf((float)y / (float)height * (float)image->height);
			src_x = MIN(src_x, image->width - 1);
			src_y = MIN(src_y, image->height - 1);

			memcpy(&_image->data[y][x * _image->bytes_per_pixel],
				   &image->data[src_y][src_x * _image->bytes_per_pixel],
				   image->bytes_per_pixel);
		}
	}

	return _image;
}

void image_processing_dithering(image_t *const image)
{
	for (size_t y = 0; y < image->height; y++) {
		for (size_t x = 0; x < image->width; x++) {
			uint8_t *pixel = &image->data[y][x * image->bytes_per_pixel];
			uint8_t qpixel = pixel[0] >= 128 ? 255 : 0;

			int32_t error = (int32_t)roundf((float)(pixel[0] - qpixel) * 0.605f);

			pixel[0] = pixel[1] = pixel[2] = qpixel;

			if (x != (image->width - 1)) {
				pixel = &image->data[y][(x + 1) * image->bytes_per_pixel];
				pixel[0] += (uint8_t)((float)error * (7.0f / 16.0f));
				pixel[1] = pixel[0];
				pixel[2] = pixel[0];
			}
	
			if (y != (image->height - 1)) {
				if (x != 0) {
					pixel = &image->data[y + 1][(x - 1) * image->bytes_per_pixel];
					pixel[0] += (uint8_t)((float)error * (3.0f / 16.0f));
					pixel[1] = pixel[0];
					pixel[2] = pixel[0];
				}
	
				pixel = &image->data[y + 1][x * image->bytes_per_pixel];
				pixel[0] += (uint8_t)((float)error * (5.0f / 16.0f));
				pixel[1] = pixel[0];
				pixel[2] = pixel[0];
	
				if (x != (image->width - 1)) {
					pixel = &image->data[y + 1][(x + 1) *  image->bytes_per_pixel];
					pixel[0] += (uint8_t)((float)error * (1.0f / 16.0f));
					pixel[1] = pixel[0];
					pixel[2] = pixel[0];
				}
			}
		}
	}
}

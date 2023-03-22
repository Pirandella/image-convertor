#include "imageHandler.h"

static void abort_(const char *s, ...)
{
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
	abort();
}

void image_handler_read(const char *const path, image_t *image)
{
	char header[8];    // 8 is the maximum size that can be checked

	/* open file and test for it being a png */
	FILE *fp = fopen(path, "rb");
	if(!fp)
		abort_("[image_handler_read] File %s could not be opened for reading", path);
	fread(header, 1, 8, fp);
	if(png_sig_cmp((png_const_bytep)header, 0, 8))
		abort_("[image_handler_read] File %s is not recognized as a PNG file", path);

	/* initialize stuff */
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if(!png_ptr)
		abort_("[image_handler_read] png_create_read_struct failed");

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr)
		abort_("[image_handler_read] png_create_info_struct failed");

	if(setjmp(png_jmpbuf(png_ptr)))
		abort_("[image_handler_read] Error during init_io");

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	// image->width = png_get_image_width(png_ptr, info_ptr);
	// image->height = png_get_image_height(png_ptr, info_ptr);
	// image->capacity = image->width;

	png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	uint8_t bytes_per_pixel = NOT_SET;
	if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGBA) {
		bytes_per_pixel = RGBA;
	} else {
		bytes_per_pixel = RGB;
	}

	/* read file */
	if(setjmp(png_jmpbuf(png_ptr)))
		abort_("[image_handler_read] Error during read_image");

	/* Allocate memory for source image data */
	// image->data = (png_bytepp)malloc(sizeof(png_bytep) * image->height);
	// for(size_t i = 0; i < image->height; i++)
	// 	image->data[i] = (png_bytep)malloc(image->width * image->bytes_per_pixel);
	image_allocate_data(image, png_get_image_width(png_ptr, info_ptr),
						png_get_image_height(png_ptr, info_ptr), bytes_per_pixel);

	/* Read source image data */
	png_read_image(png_ptr, image->data);

	/* Cleanup */
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);
}

void image_handler_write(const char *const path, image_t *image)
{
	png_structp png_ptr;
	png_infop info_ptr;
	/* create file */
	FILE *fp = fopen(path, "wb");
	if(!fp)
		abort_("[image_handler_write] File %s could not be opened for writing", path);

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr)
		abort_("[image_handler_write] Failed to create write struct");
			
	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr) {
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		abort_("[image_handler_write] Failed to create info structure");
	}

	if(setjmp(png_jmpbuf(png_ptr)))
		abort_("[image_handler_write] Error during info_io_s");
	
	png_init_io(png_ptr, fp);

	/* write header */
	if(setjmp(png_jmpbuf(png_ptr)))
		abort_("[image_handler_write] Error during writing header");

	uint8_t color_type;
	if (image->bytes_per_pixel == RGBA) {
		color_type = PNG_COLOR_TYPE_RGBA;
	} else {
		color_type = PNG_COLOR_TYPE_RGB;
	}

	png_set_IHDR(png_ptr, info_ptr, image->width, image->height, 8, color_type,
					PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);
	/* write bytes */
	if(setjmp(png_jmpbuf(png_ptr)))
		abort_("[image_handler_write] Error during writing bytes");

	png_write_image(png_ptr, image->data);

	/* end write */
	if(setjmp(png_jmpbuf(png_ptr)))
		abort_("[image_handler_write] Error during end of write");

	png_write_end(png_ptr, NULL);
	
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
}

#include "imgProcessing.h"

static uint8_t flags = 0;
static uint16_t num_grayscale;

static uint16_t input_img_width;
static uint16_t input_img_height;
static uint16_t output_img_width;
static uint16_t output_img_height;

static png_bytep *input_img_data;
static png_bytep *output_img_data;
static char *grayscale_img_data;

#define MIN(a, b) ((a < b) ? a : b)

static void write_png_file(char *path);
static void write_bin_ascii_file(char *path);

static void convert_to_grayscale(png_byte **img, uint32_t img_width, uint32_t img_height, uint8_t bit_depth)
{       
        uint8_t conversion_factor = 255 / (bit_depth - 1);
        
        for (uint32_t y = 0; y < img_height; y++) {
                png_byte *row = img[y];
                for (uint32_t x = 0; x < img_width; x++) {
                        png_byte *pixel = &(row[x * 4]);
                        uint8_t avg_color = (uint8_t)(pixel[0] * 0.299f) + (uint8_t)(pixel[1] * 0.587f) + (uint8_t)(pixel[2] * 0.114f);
                        uint8_t grayscale = (uint8_t)((avg_color / conversion_factor) + 0.5f) * conversion_factor;
                        pixel[0] = grayscale;
                        pixel[1] = grayscale;
                        pixel[2] = grayscale;
                }
        } 
}

static void rescale_image(png_byte **img, uint32_t img_w1, uint32_t img_h1, png_byte **out, uint32_t img_w2, uint32_t img_h2)
{
	/* Nearest Neighbor Image Scaling */
	for(uint32_t x = 0; x < img_w2; x++) {
			for(uint32_t y = 0; y < img_h2; y++) {
				int src_x = round((double)x / (double)img_w2 * (double)img_w1);
				int src_y = round((double)y / (double)img_h2 * (double)img_h1);
				src_x = MIN(src_x, img_w1 - 1);
				src_y = MIN(src_y, img_h1 - 1);

				memcpy(&out[y][x * 4], &img[src_y][src_x * 4], 4);
			}
	}
}

static void convert_grayscale_to_byte(png_byte **img, uint32_t w, uint32_t h)
{
	uint32_t idx = 0;

	for(uint32_t y = 0; y < w; y++) {
		png_byte *row = img[y];
		for(uint32_t x = 0; x < h; x++) {
			png_byte *pixel = &(row[x * 4]);
			grayscale_img_data[idx++] = (uint8_t)round(pixel[0] * num_grayscale / UINT8_MAX);
		}
	}
}

static void abort_(const char *s, ...)
{
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
	abort();
}

void ip_init(uint8_t f_pack, uint8_t f_verb, uint8_t f_scale, uint16_t img_w, uint16_t img_h, uint16_t n_shades)
{
	if(f_pack) {
		flags |= 1;
	}
	if(f_scale) {
		flags |= 2;
		output_img_width = img_w;
		output_img_height = img_h;
	}
	if(f_verb) {
		flags |= 4;
	}
	if(n_shades == 0) {
		num_grayscale = 16;
	} else {
		num_grayscale = n_shades;
	}
}

void ip_read(char *path)
{
	char header[8];    // 8 is the maximum size that can be checked

	/* open file and test for it being a png */
	FILE *fp = fopen(path, "rb");
	if(!fp)
		abort_("[ip_read] File %s could not be opened for reading", path);
	fread(header, 1, 8, fp);
	if(png_sig_cmp((png_const_bytep)header, 0, 8))
		abort_("[ip_read] File %s is not recognized as a PNG file", path);

	/* initialize stuff */
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if(!png_ptr)
		abort_("[ip_read] png_create_read_struct failed");

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr)
		abort_("[ip_read] png_create_info_struct failed");

	if(setjmp(png_jmpbuf(png_ptr)))
		abort_("[ip_read] Error during init_io");

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	input_img_width = png_get_image_width(png_ptr, info_ptr);
	input_img_height = png_get_image_height(png_ptr, info_ptr);
	if(!(flags & 0x02)) {
		output_img_width = input_img_width;
		output_img_height = input_img_height;
	}

	png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB)
		abort_("[ip_read] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA "
				"(lacks the alpha channel)");

	if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA)
		abort_("[ip_read] color_type of input file must be PNG_COLOR_TYPE_RGBA (%d) (is %d)",
				PNG_COLOR_TYPE_RGBA, png_get_color_type(png_ptr, info_ptr));

	/* read file */
	if(setjmp(png_jmpbuf(png_ptr)))
		abort_("[ip_read] Error during read_image");

	/* Allocate memory for input and output image data */
	input_img_data = malloc(sizeof(png_bytep) * input_img_height);
	for(int i = 0; i < input_img_height; i++)
		input_img_data[i] = malloc(input_img_width * 4);

	output_img_data = malloc(sizeof(png_bytep) * output_img_height);
	for(int i = 0; i < output_img_height; i++)
		output_img_data[i] = malloc(output_img_width * 4);

	grayscale_img_data = malloc(sizeof(uint8_t) * (output_img_height * output_img_width));

	/* Read input image data */
	png_read_image(png_ptr, input_img_data);

	fclose(fp);
}

void ip_process()
{
	png_byte **img = input_img_data;

	convert_to_grayscale(img, input_img_width, input_img_height, num_grayscale);
	if(flags & 0x02) {
		rescale_image(img, input_img_width, input_img_height,
			output_img_data, output_img_width, output_img_height);
		img = output_img_data;
	}
	convert_grayscale_to_byte(img, output_img_width, output_img_height);
}

void ip_write(char *path)
{
	if(flags & 0x04) {
		char *png_path = malloc(sizeof(char) * strlen(path) + 5);
		strcpy(png_path, path);
		
		for(uint16_t i = strlen(path); i > 0; i--) {
			if(path[i] == '.') {
				strcpy(&png_path[i], ".png");
				break;
			} else if(path[i] == '/') {
				strcat(png_path, ".png");
				break;
			} else if(i == 1) {
				strcat(png_path, ".png");
				break;
			}
		}
		write_png_file(png_path);
	}
	
	write_bin_ascii_file(path);
}

void ip_deinit()
{
	if(input_img_data != NULL) {
		for (int i = 0; i < input_img_height; i++)
			free(input_img_data[i]);
        free(input_img_data);
	}
	if(output_img_data != NULL) {
		for (int i = 0; i < output_img_height; i++)
			free(output_img_data[i]);
        free(output_img_data);
	}
	if(grayscale_img_data != NULL) {
		free(grayscale_img_data);
	}
}

static void write_png_file(char *path)
{
	png_structp png_ptr;
	png_infop info_ptr;
	/* create file */
	FILE *fp = fopen(path, "wb");
	if(!fp)
		abort_("[write_png_file] File %s could not be opened for writing", path);

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr)
		abort_("[write_png_file] Failed to create write struct");
			
	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr) {
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		abort_("[write_png_file] Failed to create info structure");
	}

	if(setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during info_io_s");
	
	png_init_io(png_ptr, fp);

	/* write header */
	if(setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing header");

	png_set_IHDR(png_ptr, info_ptr, output_img_width, output_img_height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
					PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);
	/* write bytes */
	if(setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing bytes");

	if(flags & 0x02) {
		png_write_image(png_ptr, output_img_data);
	} else {
		png_write_image(png_ptr, input_img_data);
	}

	/* end write */
	if(setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during end of write");

	png_write_end(png_ptr, NULL);

	fclose(fp);
}

static void write_bin_ascii_file(char *path)
{
	FILE *fp = fopen(path, "w");
	if(!fp)
		abort_("[write_bin_ascii_file] File %s could not be opened for writing", path);
	
	fprintf(fp, "%d\t%d\t%s\r\n", output_img_width, output_img_height, (flags & 0x01) ? "packed" : "normal");
	uint8_t p = 1;
	uint8_t byte = 0;
	for(uint32_t i = 0; i < (output_img_width * output_img_height); i += p) {
		if(flags & 0x01) {
			byte = (grayscale_img_data[i] << 4) | (grayscale_img_data[i + 1]);
			p = 2;
			fprintf(fp, "0x%02X,%c", byte, (((i + 2) % output_img_width) ? ' ' : '\n'));
		} else {
			byte = grayscale_img_data[i];
			fprintf(fp, "0x%02X,%c", byte, (((i + 1) % (output_img_width)) ? ' ' : '\n'));
		}
	}

	fclose(fp);
}
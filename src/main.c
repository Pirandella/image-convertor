#include "argParser.h"
#include "imageHandler.h"
#include "imageProcessing.h"
#include "fileHandler.h"

int main(int argc, char **argv)
{
	args *arg = ap_init(argc, argv);

	image_t *src_img = image_init();
	image_t *out_img = src_img;
	image_handler_read(arg->input_path, src_img);


	image_processing_grayscale(src_img, arg->grayscale);
	if (arg->flags & ARG_RESCALE) {
		out_img = image_processing_rescale(src_img, arg->width, arg->height);
	}
	if (arg->flags & ARG_DITHERING) {
		image_processing_dithering(out_img);
	}
	if (arg->flags & ARG_INVERT) {
		image_processing_invert(out_img);
	}
	if (arg->flags & ARG_VERBOSE) {
		file_handler_name_resolver(arg->output_path, PNG);
		image_handler_write(arg->output_path, out_img);
	}

	file_handler_write(arg->output_path, out_img, arg->flags & ARG_PACKED);

	image_free(src_img);
	if (arg->flags & ARG_RESCALE) image_free(out_img);
	ap_deinit(arg);

	return EXIT_SUCCESS;
}

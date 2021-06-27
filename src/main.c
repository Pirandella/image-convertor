#include <stdio.h>

#include "argParser.h"

int main(int argc, char **argv)
{
	struct args *arg = ap_init(argc, argv);

	printf("n_shades\t%d\nscale:\t\tflag: %d\tw: %4d\th: %4d\npack:\t\t%d\ninput_path:\t%s\noutput_path:\t%s\n", 
			arg->n_shades, arg->scale, arg->w_scale, arg->h_scale, arg->packed, arg->input_path, arg->output_path);

	ap_deinit();

	return EXIT_SUCCESS;
}
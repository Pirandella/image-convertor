#include <stdio.h>

#include "argParser.h"
#include "imgProcessing.h"

int main(int argc, char **argv)
{
	struct args *arg = ap_init(argc, argv);

	ip_init(*arg);
	ip_read(arg->input_path);
	ip_process();
	ip_write(arg->output_path);

	ap_deinit();
	ip_deinit();

	return EXIT_SUCCESS;
}
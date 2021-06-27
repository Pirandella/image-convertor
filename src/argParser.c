#include "argParser.h"

static struct args *arg;

static void print_help(void);

struct args *ap_init(int argc, char **argv)
{
	arg = malloc(sizeof(struct args));
	int opt;

	while((opt = getopt(argc, argv, "phg:s:i:o:")) != -1) {
		switch(opt) {
		case 'p':
			arg->packed = 1;
			break;
		case 'g':
			arg->n_shades = atoi(optarg);
			if(arg->n_shades < 2)
				arg->n_shades = 2;
			break;
		case 's':
			arg->scale = 1;
			arg->w_scale = atoi(optarg);
			arg->h_scale = atoi(argv[optind++]);
			break;
		case 'i':
			arg->input_path = malloc(sizeof(char) * strlen(optarg));
			strcpy(arg->input_path, optarg);
			break;
		case 'o':
			arg->output_path = malloc(sizeof(char) * strlen(optarg));
			strcpy(arg->output_path, optarg);
			break;
		case 'h':
		default:
			print_help();
			exit(EXIT_FAILURE);
		}
	}

	/* Check existence required arguments */
	if(arg->input_path == NULL || arg->output_path == NULL) {
		printf("Path to input file and path to output file is required! Use -h to get usage info\n");
		exit(EXIT_FAILURE);
	}
	/* Check if input file exist */
	if(access(arg->input_path, R_OK) != 0) {
		printf("Input file doesn't exist!\n");
		exit(EXIT_FAILURE);
	}

	return arg;
}

void ap_deinit()
{
	if(arg != NULL) {
		free(arg->input_path);
		free(arg->output_path);
		free(arg);
	}
}

static void print_help(void)
{
	puts("image converter:\n\
	-i : Path to input image\n\
	-o : Path to output file\n\
	-g : Number of shades of gray\n\
	-s : Image scale [width height]\n\
	-p : Pack two pixels in one byte (only if gray <= 16)\n\
	-h : Help");
}
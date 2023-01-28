#include "argParser.h"

static struct args *arg;

static void print_help(void);

struct args *ap_init(int argc, char **argv)
{
	arg = malloc(sizeof(struct args));
	arg->flags = 0;
	int opt;

	while((opt = getopt(argc, argv, "pvdhg:s:i:o:")) != -1) {
		switch(opt) {
		case 'p':
			arg->flags |= ARG_PACKED;
			break;
		case 'v':
			arg->flags |= ARG_VERBOSE;
			break;
		case 'd':
			arg->flags |= ARG_DITHERING;
			break;
		case 'g':
			arg->n_shades = atoi(optarg);
			if(arg->n_shades < 2)
				arg->n_shades = 2;
			break;
		case 's':
			arg->flags |= ARG_SCALE;
			arg->w_scale = atoi(optarg);
			if(!(*argv[optind] == '-')) {
				arg->h_scale = atoi(argv[optind++]);
			} else {
				/* If only one side of image size is set. Make image square */
				arg->h_scale = arg->w_scale;
			}
			break;
		case 'i':
			arg->input_path = malloc(sizeof(char) * strlen(optarg) + 1);
			strcpy(arg->input_path, optarg);
			break;
		case 'o':
			arg->output_path = malloc(sizeof(char) * strlen(optarg) + 1);
			strcpy(arg->output_path, optarg);
			break;
		case 'h':
		default:
			print_help();
			exit(EXIT_FAILURE);
		}
	}

	/* Check if input file exists */
	if(arg->input_path == NULL) {
		printf("Path to input file is required! Use -h to get usage info\n");
		exit(EXIT_FAILURE);
	}
	/* Check if output file path is specified */
	if(arg->output_path == NULL) {
		printf("Path to output file is required! Use -h to get usage info\n");
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
	-d : Apply dithering to the image\n\
	-v : Save converted png image\n\
	-h : Help");
}
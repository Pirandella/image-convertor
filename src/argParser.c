#include "argParser.h"

#define OUTPUT_FILE_EXTENTION_SIZE	(5U)

static void print_help(void);

args *ap_init(int argc, char **argv)
{
	args *arg = (args *)malloc(sizeof(args));
	arg->input_path = NULL;
	arg->output_path = NULL;
	arg->flags = 0;
	int opt;

	while((opt = getopt(argc, argv, "pvdhng:s:i:o:")) != -1) {
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
		case 'n':
			arg->flags |= ARG_INVERT;
			break;
		case 'g':
			arg->grayscale = atoi(optarg);
			if(arg->grayscale < 2)
				arg->grayscale = 2;
			break;
		case 's':
			arg->flags |= ARG_RESCALE;
			arg->width = atoi(optarg);
			if(!(*argv[optind] == '-')) {
				arg->height = atoi(argv[optind++]);
			} else {
				/* If only one side of image size is set. Make image square */
				arg->height = arg->width;
			}
			break;
		case 'i':
			arg->input_path = (char *)malloc(sizeof(char) * strlen(optarg) + 1);
			strcpy(arg->input_path, optarg);
			break;
		case 'o':
			arg->output_path = (char *)malloc(sizeof(char) * strlen(optarg) + 
											  OUTPUT_FILE_EXTENTION_SIZE);
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

	if ((arg->flags & ARG_PACKED) && (arg->grayscale > 16)) {
		arg->grayscale = 16;
	}

	return arg;
}

void ap_deinit(args *arg)
{
	if (arg == NULL) return;
	free(arg->input_path);
	free(arg->output_path);
	free(arg);
}

static void print_help(void)
{
	puts("image converter for OLED displays:\n\
	-i : Path to input image\n\
	-o : Path to output file\n\
	-g : Number of shades of gray\n\
	-s : Image scale [width height]\n\
	-p : Pack two pixels in one byte (only if gray <= 16)\n\
	-d : Apply dithering to the image\n\
	-n : Invert image colors\n\
	-v : Save converted png image\n\
	-h : Help");
}
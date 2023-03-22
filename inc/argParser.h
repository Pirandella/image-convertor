#ifndef _ARG_PARSER_H_
#define _ARG_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>

/* 
	-g : Set number of shades of gray [n]
	-s : Scale [w, h]
	-p : Pack put to pixels in one byte (if number of shades in less then 16)
	-d : Applay dithering to the image
	-n : Invert image colors
	-i : Input image [path]
	-o : Output file [path]
	-h : Help
 */

#define ARG_PACKED		(1U << 0U)
#define ARG_VERBOSE		(1U << 1U)
#define ARG_RESCALE		(1U << 2U)
#define ARG_DITHERING	(1U << 3U)
#define ARG_INVERT		(1U << 4U)

typedef struct _args {
	size_t width;
	size_t height;
	char *input_path;
	char *output_path;
	uint8_t grayscale;
	uint8_t flags;
} args;

args *ap_init(int argc, char **argv);

void ap_deinit(args *arg);

#endif /* _ARG_PARSER_H_ */
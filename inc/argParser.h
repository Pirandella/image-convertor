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
	-i : Input image [path]
	-o : Output file [path]
	-h : Help
 */

struct args {
	uint16_t n_shades;
	uint16_t w_scale;
	uint16_t h_scale;
	uint8_t packed;
	uint8_t scale;
	char *input_path;
	char *output_path;
};

struct args *ap_init(int argc, char **argv);
void ap_deinit();

#endif /* _ARG_PARSER_H_ */
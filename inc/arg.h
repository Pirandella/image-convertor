#ifndef _ARG_H_
#define _ARG_H_

#include <stdint.h>

/* 
	-g : Set number of shades of gray [n]
	-s : Scale [w, h]
	-p : Pack put to pixels in one byte (if number of shades in less then 16)
	-d : Applay dithering to the image
	-i : Input image [path]
	-o : Output file [path]
	-h : Help
 */

#define ARG_PACKED		1
#define ARG_SCALE		2
#define ARG_VERBOSE		4
#define ARG_DITHERING	8

struct args {
	uint16_t n_shades;
	uint16_t w_scale;
	uint16_t h_scale;
	uint16_t flags;
	char *input_path;
	char *output_path;
};

#endif /* _ARG_H_ */
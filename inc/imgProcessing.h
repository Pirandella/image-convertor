#ifndef _IMG_PROCESSING_H_
#define _IMG_PROCESSING_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#define PNG_DEBUG	3
#include <png.h>

void ip_init(uint8_t f_pack, uint8_t f_verb, uint8_t f_scale, uint16_t img_w, uint16_t img_h, uint16_t n_grayscale);
void ip_read(char *path);
void ip_process();
void ip_write(char *path);
void ip_deinit();

#endif /* _IMG_PROCESSING_H_ */
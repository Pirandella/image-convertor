#ifndef _IMG_PROCESSING_H_
#define _IMG_PROCESSING_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "arg.h"

#define PNG_DEBUG	3
#include <png.h>

void ip_init(struct args arg);
void ip_read(char *path);
void ip_process();
void ip_write(char *path);
void ip_deinit();

#endif /* _IMG_PROCESSING_H_ */
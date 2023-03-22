#ifndef _IMAGE_HANDLER_H_
#define _IMAGE_HANDLER_H_

#include <stdint.h>
#include <stdarg.h>

#define PNG_DEBUG	3
#include <png.h>

#include "image.h"

void image_handler_read(const char *const path, image_t *image);

void image_handler_write(const char *const path, image_t *image);

#endif /* _IMAGE_HANDLER_H_ */
#ifndef _FILE_HANDLER_H_
#define _FILE_HANDLER_H_

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>

#include "image.h"

typedef enum {
	TXT,
	H_FILE,
	PNG,
} FILE_EXTENTION;

size_t file_handler_name_resolver(char *path, FILE_EXTENTION extention);

void file_handler_write(char *path, const image_t *const image, uint8_t packed);

#endif /* _FILE_HANDLER_H_ */
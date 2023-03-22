#include "fileHandler.h"

static const char *INCLUDE_GUARD_TOP = "#ifndef _%s_H_\n#define _%s_H_\n\n";
static const size_t INCLUDE_GUARD_TOP_SIZE = 27;

static const char *ARRAY_TOP = "%s image_%s[%d][%d] = {\n";
static const size_t ARRAY_TOP_SIZE = 16;

static const char *ARRAY_BOTTOM = "};\n\n";
static const size_t ARRAY_BOTTOM_SIZE = 4;

static const char *INCLUDE_GUARD_BOTTOM = "#endif /* _%s_H_ */\n";
static const size_t INCLUDE_GUARD_BOTTOM_SIZE = 18;

static const char *_array_type[] = {
	"uint8_t",
	"uint16_t",
};

static const char *_file_extention[] = {
	".txt\0",
	".h\0",
	".png\0",
};

size_t file_handler_name_resolver(char *path, FILE_EXTENTION extention)
{
	size_t write_index = strlen(path);
	char *ch = rindex(path, '.');
	if (ch != NULL) {
		if ((ch - path) > 2)
			write_index = ch - path;
	}

	strcpy(&path[write_index], _file_extention[extention]);

	return write_index;
}

static char *str_toupper(char *str, size_t len)
{
	char *_str = (char *)calloc(1, sizeof(char) * (len + 1));

	for (size_t i = 0; i < len; i++)
		_str[i] = toupper(str[i]);

	return _str;
}

static char *create_file_content(char *path, const image_t *const image, uint8_t packed)
{
	size_t dot_index = file_handler_name_resolver(path, H_FILE);
	size_t slash_index = 0;
	char *ch = rindex(path, '/');
	if (ch != NULL) slash_index = ch - path + 1;

	size_t file_name_size = dot_index - slash_index;
	char *file_name = (char *)calloc(1, sizeof(char) * (file_name_size + 1));
	strncpy(file_name, &path[slash_index], file_name_size);


	if (packed > 1) packed = 1; 
	size_t payload_size = INCLUDE_GUARD_TOP_SIZE + (file_name_size * 2) + INCLUDE_GUARD_BOTTOM_SIZE + file_name_size +
						  ARRAY_TOP_SIZE + strlen(_array_type[packed]) + 8 + (image->height * (image->width * 7)) +
						  ARRAY_BOTTOM_SIZE + 2;

	char *file_name_capetalized = str_toupper(file_name, file_name_size);
	char *payload = (char *)calloc(1, sizeof(char) * payload_size);

	size_t image_width = image->width;
	uint8_t index_inc = 1;
	if (packed) { 
		image_width = image->width / 2;
		index_inc = 2;
	}

	sprintf(payload, INCLUDE_GUARD_TOP, file_name_capetalized, file_name_capetalized);
	sprintf(&payload[strlen(payload)], ARRAY_TOP, _array_type[packed],
			file_name, image->height, image_width);

	for (size_t i = 0; i < image->height; i++) {
		for (size_t j = 0; j <= image->width - index_inc; j += index_inc) {
			uint8_t data = image->data[i][j * image->bytes_per_pixel];
			if (packed) {
				data = (image->data[i][j * image->bytes_per_pixel] & 0xF0) |
						(image->data[i][(j + 1) * image->bytes_per_pixel] & 0x0F);
			}

			sprintf(&payload[strlen(payload)], "%c0x%02X,%c", 
					(j == 0) ? '\t' : ' ', data,
					(j == (image->width - index_inc)) ? '\n' : ' ');
		}
	}

	sprintf(&payload[strlen(payload)], ARRAY_BOTTOM);
	sprintf(&payload[strlen(payload)], INCLUDE_GUARD_BOTTOM, file_name_capetalized);

	free(file_name);
	free(file_name_capetalized);
	return payload;
}

void file_handler_write(char * path, const image_t *const image, uint8_t packed)
{
	char *payload = create_file_content(path, image, packed);

	FILE *file = fopen(path, "w");
	if (file == NULL) {
		perror("Failed to open file for write");
		exit(-1);
	}

	fwrite(payload, strlen(payload), 1, file);

	fclose(file);
	free(payload);
}
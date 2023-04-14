#include "fileHandler.h"
#include "math.h"

static const char *INCLUDE_GUARD_TOP = "#ifndef _%s_H_\n#define _%s_H_\n\n";
static const size_t INCLUDE_GUARD_TOP_SIZE = 27;

static const char *ARRAY_TOP = "uint8_t image_%s[%d][%d] = {\n";
static const size_t ARRAY_TOP_SIZE = 23;

static const char *ARRAY_BOTTOM = "};\n\n";
static const size_t ARRAY_BOTTOM_SIZE = 4;

static const char *INCLUDE_GUARD_BOTTOM = "#endif /* _%s_H_ */\n";
static const size_t INCLUDE_GUARD_BOTTOM_SIZE = 18;

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

#if 0
static void format_noraml(const image_t *const image, char *const payload)
{
	for (size_t y = 0; y < image->width; y++) {
		for (size_t x = 0; x < image->height; x++) {
			sprintf(&payload[strlen(payload)], "0x%02X, ", image->data[y][x * image->bytes_per_pixel]);
		}
		sprintf(&payload[strlen(payload)], "\n");
	}
}
#endif

static void format_ssd1322(const image_t *const image, char *const payload)
{
	for (size_t y = 0; y < image->width; y++) {
		for (size_t x = 0; x < image->height; x += 2) {
			uint8_t data = (image->data[y][x * image->bytes_per_pixel] & 0xF0) |
						   (image->data[y][(x + 1) * image->bytes_per_pixel] & 0x0F);
			sprintf(&payload[strlen(payload)], "0x%02X, ", data);
		}
		sprintf(&payload[strlen(payload)], "\n");
	}
}

static void format_ssd1306(const image_t *const image, char *const payload)
{
	size_t y = 0;
	while (y != image->height) {
		size_t offset = 0;
		for (size_t x = 0; x < image->width; x++) {
			uint8_t data = 0;
			for (; offset < 8; offset++) {
				if ((y + offset) == image->height) break;
				data |= (image->data[y + offset][x * image->bytes_per_pixel] & 0x01) << (7 - offset);
			}
			sprintf(&payload[strlen(payload)], "0x%02X, ", data);
		}
		sprintf(&payload[strlen(payload)], "\n");
		y += offset;
	}
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

	size_t image_width;
	size_t image_height;
	if (packed) {
		image_width = image->width / 2;
		image_height = image->height;
	} else {
		image_width = image->width;
		image_height = ceill(image->height / 8.0f);
	}
	size_t payload_size = INCLUDE_GUARD_TOP_SIZE  + INCLUDE_GUARD_BOTTOM_SIZE +
						  (file_name_size * 3) +
						  ARRAY_TOP_SIZE + 6 + 
						  (image_height * (image_width * 6)) + image_height +
						  ARRAY_BOTTOM_SIZE + 1;

	char *file_name_capetalized = str_toupper(file_name, file_name_size);
	char *payload = (char *)calloc(1, sizeof(char) * payload_size);

	sprintf(payload, INCLUDE_GUARD_TOP, file_name_capetalized, file_name_capetalized);
	sprintf(&payload[strlen(payload)], ARRAY_TOP, file_name, image_height, image_width);

	if (packed)
		format_ssd1322(image, payload);
	else
		format_ssd1306(image, payload);

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
#pragma once
#include <stdint.h>
#include <stdio.h>
#include "Immagine/Exports.h"

enum IMAGE_KIND
{
	KIND_NONE,
	BMP,
	PNG,
	JPEG
};

struct Image
{
	unsigned char *data;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
};

API_IMMAGINE Image
image_new();

API_IMMAGINE Image
image_new(uint32_t width, uint32_t height, uint32_t depth = 4);

API_IMMAGINE void
image_free(Image& image);

inline static void
destruct(Image& image)
{
	image_free(image);
}

API_IMMAGINE Image
image_read(const char* file_path);

API_IMMAGINE bool
image_write(const char* file_path, const Image& image, IMAGE_KIND kind);

API_IMMAGINE Image
image_binarize(const Image& image, uint8_t threshold);

API_IMMAGINE Image
image_red_channel(const Image& image);

API_IMMAGINE Image
image_green_channel(const Image& image);

API_IMMAGINE Image
image_blue_channel(const Image& image);

API_IMMAGINE Image
image_rgb_to_gray_scale(const Image& image);

API_IMMAGINE Image
image_brightness(const Image& image, int16_t brightness);

inline static void
_image_histogram_dump(const char* file_path, float data[])
{
	FILE *handle = fopen(file_path, "w");

	for (size_t i = 0; i < 256; ++i)
		fprintf(handle, "\n%f", data[i]);

	fclose(handle);
}

API_IMMAGINE void
image_histogram(const Image& image, float hist[]);

API_IMMAGINE void
image_histogram_red_channel(const Image& image, float hist[]);

API_IMMAGINE void
image_histogram_green_channel(const Image& image, float hist[]);

API_IMMAGINE void
image_histogram_blue_channel(const Image& image, float hist[]);

API_IMMAGINE Image
image_histogram_equalization(const Image& image);
#pragma once
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "Immagine/Exports.h"
namespace immagine
{
	enum IMAGE_KIND
	{
		NONE,
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

		unsigned char&
		operator()(size_t ix, size_t iy, size_t iz)
		{
			assert(ix >= 0 && ix < width  && "ERROR: Index out of range.\n");
			assert(iy >= 0 && iy < height && "ERROR: Index out of range.\n");
			assert(iz >= 0 && iz < depth  && "ERROR: Index out of range.\n");

			size_t idx = ix + iy * width + iz * width * height;
			return data[idx];
		}

		const unsigned char&
		operator()(size_t ix, size_t iy, size_t iz) const
		{
			assert(ix >= 0 && ix < width  && "ERROR: Index out of range.\n");
			assert(iy >= 0 && iy < height && "ERROR: Index out of range.\n");
			assert(iz >= 0 && iz < depth  && "ERROR: Index out of range.\n");

			size_t idx = ix + iy * width + iz * width * height;
			return data[idx];
		}
	};

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
	image_load(const char* file_path);

	API_IMMAGINE bool
	image_save(const char* file_path, const Image& image, IMAGE_KIND kind);

	API_IMMAGINE Image
	image_red_channel(const Image& image);

	API_IMMAGINE Image
	image_green_channel(const Image& image);

	API_IMMAGINE Image
	image_blue_channel(const Image& image);

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

	API_IMMAGINE Image
	image_brightness(const Image& image, int16_t brightness);

	API_IMMAGINE Image
	image_binarize(const Image& image, uint8_t threshold);

	API_IMMAGINE Image
	image_gray_scale(const Image& image);

	API_IMMAGINE Image
	image_padding(const Image& image, uint8_t expand, unsigned char value);
}
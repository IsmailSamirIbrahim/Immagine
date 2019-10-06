#pragma once

#include "Immagine/Exports.h"
#include "Immagine/Imagedef.h"

#include <assert.h>

namespace immagine
{
	struct Image
	{
		uint8_t* data;
		uint32_t width;
		uint32_t height;
		uint8_t channels;

		uint8_t&
		operator()(uint32_t row, uint32_t column)
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && "Index out of range.\n");
			return data[row * width + column];
		}

		const uint8_t&
		operator()(uint32_t row, uint32_t column) const
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && "Index out of range.\n");
			return data[row * width + column];
		}

		uint8_t&
		operator()(uint32_t row, uint32_t column, uint8_t channel)
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && channel >= 0 && "Index out of range.\n");
			return data[row * width + column + channel * width * height];
		}

		const uint8_t&
		operator()(uint32_t row, uint32_t column, uint8_t channel) const
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && channel >= 0 && "Index out of range.\n");
			return data[row * width + column + channel * width * height];
		}
	};

	IMMAGINE_EXPORT Image
	image_new(uint32_t width, uint32_t height, uint8_t channels);

	IMMAGINE_EXPORT Image
	image_clone(const Image& image);

	IMMAGINE_EXPORT Image
	image_from_ptr(const void* data, uint32_t width, uint32_t height, uint8_t channels);

	IMMAGINE_EXPORT void
	image_free(Image& image);

	inline static void
	destruct(Image& image)
	{
		image_free(image);
	}

	IMMAGINE_EXPORT Image
	image_load(const char* file_path);

	IMMAGINE_EXPORT bool
	image_save(const char* file_path, const Image& image, IMAGE_FORMAT format);

	/*Get specific channel*/
	IMMAGINE_EXPORT Image
	image_red_channel(const Image& image);

	IMMAGINE_EXPORT Image
	image_green_channel(const Image& image);

	IMMAGINE_EXPORT Image
	image_blue_channel(const Image& image);

	IMMAGINE_EXPORT Image
	image_alpha_channel(const Image& image);

	/*Basic Transformations*/
	IMMAGINE_EXPORT Image
	image_grayscale(const Image& image);

	IMMAGINE_EXPORT Image
	image_flip_horizontally(const Image& image);

	IMMAGINE_EXPORT Image
	image_flip_vertically(const Image& image);

	IMMAGINE_EXPORT Image
	image_mirror(const Image& image);

	IMMAGINE_EXPORT Image
	image_rotate_right(const Image& image);

	IMMAGINE_EXPORT Image
	image_rotate_left(const Image& image);

	IMMAGINE_EXPORT Image
	image_resize(const Image& image, uint32_t width, uint32_t height, INTERPOLATION_METHOD method);

	IMMAGINE_EXPORT Image
	image_crop(const Image& image, const Rectangle& rect);

	inline static Image
	image_crop(const Image& image, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		image_crop(image, Rectangle{ x, y, width, height });
	}

	IMMAGINE_EXPORT Image
	image_pad(const Image& image, uint32_t expand_w, uint32_t expand_h, uint8_t val);

	IMMAGINE_EXPORT Image
	image_binarize(const Image& image);
}
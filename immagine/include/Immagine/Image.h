#pragma once

#include "Immagine/Exports.h"
#include "Immagine/Imagedef.h"

namespace immagine
{
	struct Image
	{
		Byte* data;
		size_t width;
		size_t height;
		uint8_t channels;

		Byte&
		operator()(size_t row, size_t column)
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && "Index out of range.\n");
			return data[row * width + column];
		}

		const Byte&
		operator()(size_t row, size_t column) const
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && "Index out of range.\n");
			return data[row * width + column];
		}

		Byte&
		operator()(size_t row, size_t column, uint8_t channel)
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && channel >= 0 && "Index out of range.\n");
			return data[row * width + column + channel * width * height];
		}

		const Byte&
		operator()(size_t row, size_t column, uint8_t channel) const
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && channel >= 0 && "Index out of range.\n");
			return data[row * width + column + channel * width * height];
		}
	};

	IMMAGINE_EXPORT Image
	image_new(size_t width, size_t height, uint8_t channels = 1);

	IMMAGINE_EXPORT Image
	image_clone(const Image& image);

	IMMAGINE_EXPORT Image
	image_from_ptr(const void* data, size_t width, size_t height, uint8_t channels);

	IMMAGINE_EXPORT void
	image_free(Image& image);

	IMMAGINE_EXPORT void
	image_set_pixel(Image& image, size_t row, size_t column, COLOR color);

	inline static void
	destruct(Image& image)
	{
		image_free(image);
	}

	IMMAGINE_EXPORT Image
	image_load(const char* file_path);

	IMMAGINE_EXPORT bool
	image_save(const char* file_path, const Image& image, IMAGE_FORMAT kind);

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
	image_gray_scale(const Image& image);

	IMMAGINE_EXPORT Image
	image_binarize(const Image& image);

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
	image_resize(const Image& image, size_t width, size_t height, SCALLING_ALGORITHM algorithm = NEAREST_NEIGHBOUR);

	IMMAGINE_EXPORT Image
	image_crop(const Image& image, size_t x, size_t y, size_t width, size_t height);

}
#pragma once

#include "Immagine/Exports.h"
#include "Immagine/Imagedef.h"

namespace immagine
{
	struct Image
	{
		Byte* data;
		uint16_t width;
		uint16_t height;
		uint8_t channels;

		Byte&
		operator()(uint16_t row, uint16_t column)
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && "Index out of range.\n");
			return data[row * width + column];
		}

		const Byte&
		operator()(uint16_t row, uint16_t column) const
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && "Index out of range.\n");
			return data[row * width + column];
		}

		Byte&
		operator()(uint16_t row, uint16_t column, uint8_t channel)
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && channel >= 0 && "Index out of range.\n");
			return data[row * width + column + channel * width * height];
		}

		const Byte&
		operator()(uint16_t row, uint16_t column, uint8_t channel) const
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && channel >= 0 && "Index out of range.\n");
			return data[row * width + column + channel * width * height];
		}
	};

	IMMAGINE_EXPORT Image
	image_new(uint16_t width, uint16_t height, uint8_t channels = 1);

	IMMAGINE_EXPORT Image
	image_clone(const Image& image);

	IMMAGINE_EXPORT Image
	image_from_ptr(const void* data, uint16_t width, uint16_t height, uint8_t channels);

	IMMAGINE_EXPORT void
	image_free(Image& image);

	IMMAGINE_EXPORT void
	image_set_pixel(Image& image, uint16_t row, uint16_t column, Color color);

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
	image_resize(const Image& image, uint16_t width, uint16_t height, SCALLING_ALGORITHM algorithm = NEAREST_NEIGHBOUR);

	IMMAGINE_EXPORT Image
	image_crop(const Image& image, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

}
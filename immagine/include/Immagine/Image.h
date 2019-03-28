#pragma once

#include "Immagine/Exports.h"
#include "Immagine/Imagedef.h"

namespace immagine
{
	struct Image
	{
		Byte* data;
		uint32_t width;
		uint32_t height;
		uint8_t channels;

		Byte&
		operator()(size_t row, size_t column)
		{
			assert(row >= 0 && row < width && column >= 0 && column < height && "Index out of range.\n");	
			return data[row * width + column];
		}

		const Byte&
		operator()(size_t row, size_t column) const
		{
			assert(row >= 0 && row < width && column >= 0 && column < height && "Index out of range.\n");
			return data[row * width + column];
		}
	};

	API_IMMAGINE Image
	image_new(uint32_t width, uint32_t height, uint8_t channels = 4);

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
	image_save(const char* file_path, const Image& image, IMAGE_FORMAT kind);

	API_IMMAGINE Image
	image_clone(const Image& image);

	API_IMMAGINE Image
	image_from_buffer(uint32_t width, uint32_t height, uint8_t channels, const Byte* data);

	/*Get specific channel*/
	API_IMMAGINE Image
	image_red_channel(const Image& image);

	API_IMMAGINE Image
	image_green_channel(const Image& image);

	API_IMMAGINE Image
	image_blue_channel(const Image& image);

	/*Get specific histogram*/
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

	/*Pixel Operations*/
	API_IMMAGINE Image
	image_brightness(const Image& image, int16_t brightness);

	API_IMMAGINE Image
	image_binarize(const Image& image);

}
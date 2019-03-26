#pragma once
#include <stdint.h>
#include <assert.h>

#include "Immagine/Exports.h"
namespace immagine
{
	typedef unsigned char Byte;

	enum IMAGE_FORMAT
	{
		NONE,
		BMP,
		PNG,
		JPEG
	};

	struct Image
	{
		Byte* data;
		uint32_t width;
		uint32_t height;
		uint32_t depth;

		// operator overloading for 2D image
		Byte&
		operator()(size_t ix, size_t iy)
		{
			assert(ix >= 0 && ix < width && iy >= 0 && iy < height && "ERROR: Index out of range.\n");
			size_t idx = ix + iy * width;
			return data[idx];
		}

		const Byte&
		operator()(size_t ix, size_t iy) const
		{
			assert(ix >= 0 && ix < width && iy >= 0 && iy < height && "ERROR: Index out of range.\n");
			size_t idx = ix + iy * width;
			return data[idx];
		}

		// operator overloading for 3D image
		Byte&
		operator()(size_t ix, size_t iy, size_t iz)
		{
			assert(ix >= 0 && ix < width && iy >= 0 && iy < height && iz >= 0 && iz < depth && "ERROR: Index out of range.\n");
			size_t idx = ix + iy * width + iz * width * height;
			return data[idx];
		}

		const Byte&
		operator()(size_t ix, size_t iy, size_t iz) const
		{
			assert(ix >= 0 && ix < width && iy >= 0 && iy < height && iz >= 0 && iz < depth && "ERROR: Index out of range.\n");
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
	image_save(const char* file_path, const Image& image, IMAGE_FORMAT kind);

	API_IMMAGINE Image
	image_clone(const Image& image);

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
	image_binarize(const Image& image);

	API_IMMAGINE Image
	image_padding(const Image& image, uint8_t expand, Byte value);

	/*BasicTransformations*/
	API_IMMAGINE Image
	image_gray_scale(const Image& image);

	API_IMMAGINE Image
	image_flip_vertically(const Image& image);
	//Flip
	//Mirror
	//Negative
	//RotateLeft
	//RotateRight	
}
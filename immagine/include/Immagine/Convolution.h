#pragma once

#include "Immagine/Exports.h"
#include "Immagine/Image.h"

namespace immagine
{
	IMMAGINE_EXPORT Image
	image_horizental_filter(const Image& image, size_t kernel_width);

	IMMAGINE_EXPORT Image
	image_vertical_filter(const Image& image, size_t kernel_height);

	IMMAGINE_EXPORT Image
	image_box_filter(const Image& image, size_t kernel_width, size_t kernel_height);

	IMMAGINE_EXPORT Image
	image_median_filter(const Image& image, size_t kernel_width, size_t kernel_height);

	IMMAGINE_EXPORT Image
	image_gaussian_filter(const Image& image, float standard_deviation);
}
#pragma once

#include "Immagine/Exports.h"
#include "Immagine/Image.h"

namespace immagine
{
	IMMAGINE_EXPORT Image
	image_bright_increase(const Image& image, uint8_t value);

	IMMAGINE_EXPORT Image
	image_bright_decrease(const Image& image, uint8_t value);

	IMMAGINE_EXPORT Image
	image_negative(const Image& image);

	IMMAGINE_EXPORT Image
	image_logarithm(const Image& image);

	IMMAGINE_EXPORT Image
	image_gamma(const Image& image, float gamma);

	IMMAGINE_EXPORT Image
	image_gamma_correction(const Image& image, float gamma);
}
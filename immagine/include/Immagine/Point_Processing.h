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
}
#pragma once

#include "Immagine/Exports.h"
#include "Immagine/Imagedef.h"
#include "Immagine/Image.h"

namespace immagine
{
	struct Mask
	{
		float* data;
		uint8_t width;
		uint8_t height;

		float&
		operator()(uint8_t row, uint8_t column)
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && "Index out of range.\n");
			return data[row * width + column];
		}

		const float&
		operator()(uint8_t row, uint8_t column) const
		{
			assert(row >= 0 && row < height && column >= 0 && column < width && "Index out of range.\n");
			return data[row * width + column];
		}
	};

	IMMAGINE_EXPORT Mask
	mask_new(uint8_t width, uint8_t height);

	IMMAGINE_EXPORT Mask
	mask_generate(uint8_t size, IMAGE_FILTERS type);

	IMMAGINE_EXPORT void
	maske_free(Mask mask);
}
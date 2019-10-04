#pragma once

#include "Immagine/Exports.h"

#include <stdint.h>
#include <assert.h>

namespace immagine
{
	struct Kernel
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

	IMMAGINE_EXPORT Kernel
	kernel_new(uint8_t width, uint8_t height);

	IMMAGINE_EXPORT Kernel
	kernel_from_ptr(const void* data, uint8_t width, uint8_t height);

	IMMAGINE_EXPORT void
	kernel_free(Kernel& kernel);

	IMMAGINE_EXPORT Kernel
	kernel_box_gen(size_t width, size_t height);

	IMMAGINE_EXPORT int*
	kernel_gaussian_gen(float standard_deviation, size_t n);

}
#pragma once

#include <stdint.h>

namespace immagine
{

#ifndef BLACK
#define BLACK 0
#endif // !BLACK

#ifndef WHITE
#define WHITE 255
#endif // !WHITE


	enum class FORMAT { BMP, PNG, JPEG };

	enum class INTERPOLATION_METHOD { NEAREST_NEIGHBOUR, BILINEAR, BICUBIC };

	struct RGB{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	struct RGBA{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};

	struct Rectangle
	{
		uint32_t x;
		uint32_t y;
		uint32_t width;
		uint32_t height;
	};
}
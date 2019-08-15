#pragma once
#include <stdint.h>
#include <assert.h>

namespace immagine
{

#ifndef BLACK
#define BLACK 0
#endif // !BLACK

#ifndef WHITE
#define WHITE 255
#endif // !WHITE

#ifndef RED
#define RED COLOR{255, 0, 0}
#endif // !RED

#ifndef GREEN
#define GREEN COLOR{0, 255, 0}
#endif // !GREEN

#ifndef BLUE
#define BLUE COLOR{0, 0, 255}
#endif // !BLUE

	enum class IMAGE_FORMAT { BMP, PNG, JPEG };

	enum class SCALLING_ALGORITHM { NEAREST_NEIGHBOUR, BILINEAR, BICUBIC };

	enum class IMAGE_FILTERS
	{
		MEAN_FILTER,
		BOX_FILTER,
		AVG_FILTER,
		WEIGHTED_AVG_FILTER,
		GAUSSIAN_FILTER
	};

	typedef struct TAG_RGB_COLOR {
		uint8_t r;
		uint8_t g;
		uint8_t b;
	}RGB_COLOR;

	typedef struct TAG_RGBA_COLOR {
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	}RGBA_COLOR;

	typedef struct TAG_COLOR
	{
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	}COLOR;

}
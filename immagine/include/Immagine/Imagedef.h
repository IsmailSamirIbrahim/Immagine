#pragma once
#include <stdint.h>
#include <assert.h>

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

typedef unsigned char Byte;

enum IMAGE_FORMAT
{
	BMP,
	PNG,
	JPEG
};

enum SCALLING_ALGORITHM
{
	NEAREST_NEIGHBOUR,
	BILINEAR,
	BICUBIC
};

enum IMAGE_FILTERS
{
	MEAN_FILTER,
	BOX_FILTER,
	AVG_FILTER,
	WEIGHTED_AVG_FILTER,
	GAUSSIAN_FILTER
};

typedef struct TAG_COLOR
{
	Byte red;
	Byte green;
	Byte blue;
}COLOR;
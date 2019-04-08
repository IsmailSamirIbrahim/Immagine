#pragma once
#include <stdint.h>
#include <assert.h>

#ifndef BLACK
#define BLACK 0
#endif // !BLACK

#ifndef WHITE
#define WHITE 255
#endif // !WHITE

typedef unsigned char Byte;

enum IMAGE_FORMAT
{
	NONE,
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
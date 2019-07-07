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
#define RED Color{255, 0, 0}
#endif // !RED

#ifndef GREEN
#define GREEN Color{0, 255, 0}
#endif // !GREEN

#ifndef BLUE
#define BLUE Color{0, 0, 255}
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

typedef struct TAG_COLOR
{
	Byte red;
	Byte green;
	Byte blue;
}Color;

typedef struct TAG_REGION
{
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
}Region;
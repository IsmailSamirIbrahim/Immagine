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

enum class IMAGE_FORMAT
{
	NONE,
	BMP,
	PNG,
	JPEG
};
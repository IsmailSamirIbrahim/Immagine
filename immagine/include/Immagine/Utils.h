#pragma once

#include "Immagine/Image.h"
#include "Immagine/Imagedef.h"

namespace immagine
{
	inline static int8_t
	node_is_leaf(const Image& image, Region region)
	{
		for (size_t i = region.y; i < region.y + region.height; ++i)
			for (size_t j = region.x; j < region.x + region.width; ++j)
				if (image(i, j, 0) != image(region.y, region.x, 0))
					return -1;

		return image(region.y, region.x, 0) == 255 ? 1 : 0;
	}

	inline static void
	draw_horizental_line(Image& image, size_t x0, size_t y0, size_t x1, size_t y1)
	{
		for (size_t j = x0; j < x1; ++j)
		{
			if (x0 == image.width)
				--x0;
			if (y0 == image.height)
				--y0;

			image(y0, j, 0) = 255;
			image(y0, j, 1) = 0;
			image(y0, j, 2) = 0;
		}
	}

	inline static void
	draw_vertical_line(Image& image, size_t x0, size_t y0, size_t x1, size_t y1)
	{
		for (size_t i = y0; i < y1; ++i)
		{
			if (x0 == image.width)
				--x0;
			if (y0 == image.height)
				--y0;

			image(i, x0, 0) = 255;
			image(i, x0, 1) = 0;
			image(i, x0, 2) = 0;
		}
	}

	inline static void
	draw_regione(Image& image, Region region)
	{
		draw_horizental_line(image, region.x, region.y, region.x + region.width, region.y);
		draw_horizental_line(image, region.x, region.y + region.height, region.x + region.width, region.y + region.height);

		draw_vertical_line(image, region.x, region.y, region.x, region.y + region.height);
		draw_vertical_line(image, region.x + region.width, region.y, region.x + region.width, region.y + region.height);
	}

}
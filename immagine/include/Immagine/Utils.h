#pragma once

#include "Immagine/Image.h"
#include "Immagine/Imagedef.h"

#include <algorithm>

namespace immagine
{

	inline static Color
	regione_average_color(const Image& image, Region region)
	{
		uint32_t color[3] = { 0, 0, 0 };

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = region.y; i < region.y + region.height; ++i)
				for (size_t j = region.x; j < region.x + region.width; ++j)
					color[k] += image(i, j, k);

		uint32_t region_area = region.width * region.height;

		return Color{ Byte(color[0] / region_area), Byte(color[1] / region_area), Byte(color[2] / region_area) };
	}

	inline static float
	region_measure_detail(const Image& image, Region region)
	{
		Color average_color = regione_average_color(image, region);

		Byte color[3] = { average_color.red, average_color.green, average_color.blue };

		float manhattan_distance = 0.0f;

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = region.y; i < region.y + region.height; ++i)
				for (size_t j = region.x; j < region.x + region.width; ++j)
					manhattan_distance += std::abs(color[k] - image(i, j, k));

		float average_manhattan_distance = manhattan_distance / (3 * region.width * region.height);
		
		return average_manhattan_distance;
	}

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
#pragma once

#include "Immagine/Image.h"
#include <algorithm>

namespace immagine
{
	Color
	region_average_color(const Image& image, Region region)
	{
		uint32_t red_sum = 0;
		uint32_t green_sum = 0;
		uint32_t blue_sum = 0;

		//Adds the color values for each channel.
		for (size_t i = region.y; i < region.y + region.height; ++i)
			for (size_t j = region.x; j < region.x + region.width; ++j)
			{
				red_sum += image(i, j, 0);
				green_sum += image(i, j, 1);
				blue_sum += image(i, j, 2);
			}

		//Number of pixels evaluated
		uint32_t pixel_count = region.width * region.height;

		//Returns the color that represent the region color average.
		return Color{ Byte(red_sum / pixel_count), Byte(green_sum / pixel_count), Byte(blue_sum / pixel_count) };
	}

	float
	region_measure_detail(const Image& image, Region region)
	{
		Color average_color = region_average_color(image, region);

		uint32_t color_sum = 0;

		// Calculates the distance between every pixel in the region
		// and the average color. The Manhattan distance is used, and
		// all the distances are added.
		for (size_t i = region.y; i < region.y + region.height; ++i)
			for (size_t j = region.x; j < region.x + region.width; ++j)
			{
				color_sum += std::abs(image(i, j, 0) - average_color.red);
				color_sum += std::abs(image(i, j, 1) - average_color.green);
				color_sum += std::abs(image(i, j, 2) - average_color.blue);
			}

		// Calculates the average distance, and returns the result.
		uint32_t pixel_count = region.width * region.height;
		float average_distance = color_sum / (3 * pixel_count);

		return average_distance;
	}
}
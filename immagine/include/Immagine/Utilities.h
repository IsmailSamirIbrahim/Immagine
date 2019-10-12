#pragma once

#include "Immagine/Image.h"
#include "Immagine/Kernel.h"

#include <stdint.h>
#include <vector>
#include <map>

using namespace std;

namespace immagine
{
	inline static void
	calculate_summed_area(const Image& image, vector<vector<vector<uint32_t>>>& summed_table)
	{
		uint32_t value = 0;
		for (uint8_t k = 0; k < image.channels; ++k) {
			for (size_t i = 0; i < image.height; ++i) {
				for (size_t j = 0; j < image.width; ++j) {
					value = image(i, j, k);
					/* compute summed-area table value, avoid out of bounce access */
					if (i > 0)
						value += summed_table[i - 1][j][k];
					if (j > 0)
						value += summed_table[i][j - 1][k];
					if (j > 0 && i > 0)
						value -= summed_table[i - 1][j - 1][k];
					/* save computed value in summed-area table */
					summed_table[i][j][k] = value;
				}
			}
		}
	}

	inline static uint32_t
	calculate_mean(size_t i, size_t j, size_t k, size_t kernel_height, size_t kernel_width, const vector<vector<vector<uint32_t>>>& summed_table)
	{
		uint32_t mean = 0;
		if (i > 0 && j > 0)
			mean += summed_table[i - 1][j - 1][k];
		if (j > 0)
			mean -= summed_table[i - 1 + kernel_height][j - 1][k];
		if (i > 0)
			mean -= summed_table[i - 1][j - 1 + kernel_width][k];

		mean += summed_table[i + kernel_height - 1][j + kernel_width - 1][k];
		mean /= (kernel_width * kernel_height);

		return mean;
	}

	inline static uint8_t
	median_get(const map<uint8_t, size_t>& hist, size_t middle)
	{
		map<uint8_t, size_t>::const_iterator it = hist.begin();
		size_t sum = 0;
		for (; it != hist.end(); ++it)
		{
			sum += it->second;
			if (sum >= middle)
				break;
		}
		return it->first;
	}

	inline static void
	hist_add(map<uint8_t, size_t>& hist, uint8_t val)
	{
		++hist[val];
	}

	inline static void
	hist_remove(map<uint8_t, size_t>& hist, uint8_t val)
	{
		map<uint8_t, size_t>::iterator it = hist.find(val);
		if (it->second >= 1)
			--it->second;
	}

	inline static void
	window_reset(map<uint8_t, size_t>& hist, const Image& image, size_t kernel_width, size_t kernel_height, size_t i, size_t k)
	{
		hist.clear();
		for (size_t r = 0; r < kernel_height; ++r)
			for (size_t c = 0; c < kernel_width; ++c)
				hist_add(hist, image(r + i, c, k));
	}

}
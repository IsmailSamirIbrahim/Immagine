#include "Immagine/Point_Processing.h"
#include "Immagine/Utilities.h"

#include <algorithm>

#include <vector>

using namespace std;

typedef vector<vector<vector<uint32_t>>> vec3ui;
typedef vector<vector<uint32_t>> vec2ui;
typedef vector<uint32_t> vec1ui;

namespace immagine
{
	// Helper functions
	uint8_t
	truncate(uint32_t value)
	{
		if (value < 0) 
			return 0;
		if (value > 255)
			return 255;
		return value;
	}


	// API
	Image
	image_bright_increase(const Image& image, uint8_t value)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = 0; i < self.height; ++i)
				for (size_t j = 0; j < self.width; ++j)
					self(i, j, k) = (image(i, j, k) + value) > 255 ? 255 : image(i, j, k) + value;

		return self;
	}

	Image
	image_bright_decrease(const Image& image, uint8_t value)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = 0; i < self.height; ++i)
				for (size_t j = 0; j < self.width; ++j)
					self(i, j, k) = (image(i, j, k) - value) < 0 ? 0 : image(i, j, k) - value;

		return self;
	}

	Image
	image_contrast_increase(const Image& image, uint8_t value)
	{
		Image self = image_new(image.width, image.height, image.channels);

		float factor = (259.0 * (value + 255.0)) / (255.0 * (259.0 - value));

		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = 0; i < self.height; ++i)
				for (size_t j = 0; j < self.width; ++j)
					self(i, j, k) = truncate((uint32_t)(factor * (image(i, j, k) - 128) + 128));

		return self;
	}

	Image
	image_negative(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = 0; i < self.height; ++i)
				for (size_t j = 0; j < self.width; ++j)
					self(i, j, k) = (255 - image(i, j, k)) < 0 ? 0 : (255 - image(i, j, k));

		return self;
	}

	Image
	image_logarithm(const Image & image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		//find maxmim intensity
		int16_t R = INT16_MIN;
		size_t size = image.width * image.height * image.channels;
		for (size_t i = 0; i < size; ++i)
			if (image.data[i] > R)
				R = image.data[i];

		const uint8_t c = 255 / (log(1 + R));

		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = 0; i < self.height; ++i)
				for (size_t j = 0; j < self.width; ++j) {
					uint16_t val = (c * std::log(1 + image(i, j, k)));
					self(i, j, k) = (val) > 255 ? 255 : val;
				}

		return self;
	}

	Image
	image_gamma(const Image& image, float gamma)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height; ++i)
				for (size_t j = 0; j < image.width; ++j) {
					float tmp = (float(image(i, j, k)) / 255.0f);
					uint8_t val = 255 * powf(tmp, gamma);
					self(i, j, k) = val;
				}

		return self;
	}

	Image
	image_gamma_correction(const Image& image, float gamma)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height; ++i)
				for (size_t j = 0; j < image.width; ++j) {
					float tmp = (float(image(i, j, k)) / 255.0f);
					uint8_t val = 255 * powf(tmp, 1 / gamma);
					self(i, j, k) = val;
				}

		return self;
	}

	Image
	image_adaptive_threshold(const Image & image)
	{
		Image self = image_new(image.width, image.height, 1);

		float S = image.width / 8;
		float s2 = S / 2;
		float T = 1;

		vec3ui summed_table(image.height, vec2ui(image.width, vec1ui(image.channels)));
		calculate_summed_area(image, summed_table);

		for(size_t i = 0; i < image.height; ++i)
			for (size_t j = 0; j < image.width; ++j)
			{
				int x0 = std::max(int(j - s2), int(0));
				int y0 = std::max(int(i - s2), int(0));
				int y1 = std::min(int(i + s2), int(image.height - 1));
				int x1 = std::min(int(j + s2), int(image.width - 1));

				size_t count = (y1 - y0) * (x1 - x0);

				size_t sum_ = summed_table[y1][x1][0] - summed_table[y0][x1][0] - summed_table[y1][x0][0] + summed_table[y0][x0][0];

				if ((image(i, j) * count) < (sum_ * (100.0 - T) / 100.0))
					self(i, j) = 0;
				else
					self(i, j) = 255;
			}

		return self;
	}

}
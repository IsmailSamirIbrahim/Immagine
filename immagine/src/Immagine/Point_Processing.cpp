#include "Immagine/Point_Processing.h"

#include <cmath>

#include <vector>

using namespace std;

typedef vector<vector<vector<int long>>> vec3i;
typedef vector<vector<int long>> vec2i;
typedef vector<int long> vec1i;

namespace immagine
{
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

}
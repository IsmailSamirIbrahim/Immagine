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

	Image
	image_logarithm_inv(const Image & image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		//find maxmim intensity
		int16_t R = INT16_MIN;
		size_t size = image.width * image.height * image.channels;
		for (size_t i = 0; i < size; ++i)
			if (image.data[i] > R)
				R = image.data[i];

		const float c = (log(1 + R)) / 255.0;

		for (uint8_t k = 0; k < self.channels; ++k)	
			for (size_t i = 0; i < self.height; ++i)
				for (size_t j = 0; j < self.width; ++j) {
					size_t val = (std::exp(1 + image(i, j, k) * c));
					//val = (val) < 0 ? 0 : val;
					self(i, j, k) = uint8_t(val);
				}

		return self;
	}

	inline static Image
	contrast(const vector<vector<vector<int long>>>& tmp)
	{
		Image self = image_new(tmp[0].size(), tmp.size(), tmp[0][0].size());

		vector<int long> min(tmp[0][0].size(), INT_MAX);
		vector<int long> max(tmp[0][0].size(), INT_MIN);

		for (uint8_t k = 0; k < tmp[0][0].size(); ++k)
			for (size_t i = 0; i < tmp.size(); ++i)
				for (size_t j = 0; j < tmp[0].size(); ++j)
				{
					if (tmp[i][j][k] < min[k])
						min[k] = tmp[i][j][k];

					if (tmp[i][j][k] > max[k])
						max[k] = tmp[i][j][k];
				}


		for (uint8_t k = 0; k < tmp[0][0].size(); ++k)
			for (size_t i = 0; i < tmp.size(); ++i)
				for (size_t j = 0; j < tmp[0].size(); ++j) {
					int long a = (tmp[i][j][k] - min[k]);
					int long b = (max[k] - min[k]);
					int long val = int long((float(a) / float(b)) * float(255));
					self(i, j, k) = uint8_t(val);
				}

		return self;
	}

	Image
	image_gamma(const Image& image, float gamma)
	{
		vec3i tmp(image.height, vec2i(image.width, vec1i(image.channels)));

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height; ++i)
				for (size_t j = 0; j < image.width; ++j) {
					tmp[i][j][k] = powf(image(i, j, k), gamma);
				}

		return contrast(tmp);
	}

}
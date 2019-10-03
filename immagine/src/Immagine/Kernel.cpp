#include "Immagine/Kernel.h"
#include <memory>
#include <vector>
#include <omp.h>

using namespace std;

namespace immagine
{
	Kernel
	kernel_new(uint8_t width, uint8_t height)
	{
		Kernel self{};

		self.width = width;
		self.height = height;
		self.data = (float*)::malloc(width * height * sizeof(float));

		return self;
	}

	Kernel
	kernel_from_ptr(const void* data, uint8_t width, uint8_t height)
	{
		Kernel self = kernel_new(width, height);

		::memcpy(self.data, data, width * height * sizeof(float));

		return self;
	}

	void
	kernel_free(Kernel& kernel)
	{
		if (kernel.data)
			::free(kernel.data);
		kernel.data = nullptr;
	}

	inline static void
	kernel_conv_horizental(const Kernel& kernel, const Image& image, Image& self)
	{
		uint32_t offset = kernel.width / 2;

		Image padded_image = image_pad(image, offset, 0, 0);

		size_t nh = padded_image.height - (kernel.height / 2);
		size_t nw = padded_image.width - (kernel.width / 2);

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = (kernel.height / 2); i < nh; ++i)
				for (size_t j = (kernel.width / 2); j < nw; ++j) {
					float val = 0.0f;
					for (size_t x = 0; x < kernel.width; ++x)
						val += padded_image(i, j + x - kernel.width / 2, k) * kernel.data[x];
					self(i, j - offset, k) = uint8_t(val);
				}
	}

	inline static void
	kernel_conv_vertical(const Kernel& kernel, const Image& image, Image& self)
	{
		uint32_t offset = kernel.height / 2;
		Image padded_image = image_pad(image, 0, offset, 0);

		size_t nh = padded_image.height - (kernel.height / 2);
		size_t nw = padded_image.width - (kernel.width / 2);

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = (kernel.height / 2); i < nh; ++i)
				for (size_t j = (kernel.width / 2); j < nw; ++j) {
					float val = 0.0f;
					for (size_t x = 0; x < kernel.height; ++x)
						val += padded_image(i + x - kernel.height / 2, j, k) * kernel.data[x];
					self(i - offset, j, k) = uint8_t(val);
				}
	}

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

	Image
	kernel_conv(const Kernel& kernel, const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		vector<vector<vector<uint32_t>>> summed_table(image.height, vector<vector<uint32_t>>(image.width, vector<uint32_t>(image.channels)));
		calculate_summed_area(image, summed_table);

		size_t width_offset = kernel.width / 2;
		size_t height_offset = kernel.height / 2;

		for (int8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height - kernel.height; ++i)
				for (size_t j = 0; j < image.width - kernel.width; ++j)
					self(i + height_offset, j + width_offset, k) = calculate_mean(i, j, k, kernel.height, kernel.width, summed_table);

		return self;
	}
}
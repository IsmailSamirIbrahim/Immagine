#include "Immagine/Kernel.h"
#include <memory>
#include <vector>

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
	calculate_summed_area_table(vector<vector<uint32_t>>& summed_table, const Image& image)
	{
		int value = 0;
		for (size_t i = 0; i < image.height; ++i) {
			for (size_t j = 0; j < image.width; ++j) {
				value = image(i, j);
				if (j > 0)
					value += summed_table[i][j - 1];
				if (i > 0)
					value += summed_table[i - 1][j];
				if (j > 0 && i > 0)
					value -= summed_table[i - 1][j - 1];
				summed_table[i][j] = value;
			}
		}
	}

	inline static size_t
	calculate_mean(const vector<vector<uint32_t>>& summed_table, size_t i, size_t j, size_t height, size_t width, size_t x, size_t y)
	{
		int A = 0, B = 0, C = 0, D = 0;

		if (i > x && j > x)
			A = summed_table[i - y][j - y];
		if (i > x)
			B = summed_table[i - y][j + width - y];
		if (j > x)
			C = summed_table[i + height - y][j - y];

		D = summed_table[i + height - y][j + width - y];

		size_t sum = A - B - C + D;

		return sum;
	}

	Image
	kernel_conv(const Kernel& kernel, const Image& image)
	{

		vector<vector<uint32_t>> summed_table;

		summed_table.resize(image.height);
		for (size_t i = 0; i < image.height; ++i)
			summed_table[i].resize(image.width);

		calculate_summed_area_table(summed_table, image);

		Image self = image_new(image.width, image.height, image.channels);

		size_t offset = ::ceil((float)kernel.width / 2.0f);
		size_t offset1 = kernel.width / 2;

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height - offset; ++i)
				for (size_t j = 0; j < image.width - offset; ++j) {
					float val = calculate_mean(summed_table, i, j, kernel.height, kernel.width, offset1, offset);
					self(i, j, k) = uint8_t(val / (kernel.width * kernel.height));
				}

		return self;
	}
}
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
	calculate_summed_area_table(vector<vector<uint32_t>>& summedAreaTableI, const Image& image)
	{
		int value = 0;
		for (size_t i = 0; i < image.height; ++i) {
			for (size_t j = 0; j < image.width; ++j) {
				value = image(i, j);
				if (j > 0)
					value += summedAreaTableI[i][j - 1];
				if (i > 0)
					value += summedAreaTableI[i - 1][j];
				if (j > 0 && i > 0)
					value -= summedAreaTableI[i - 1][j - 1];
				summedAreaTableI[i][j] = value;
			}
		}
	}

	inline static size_t
	calculate_mean(const vector<vector<uint32_t>>& summedAreaTableI, size_t rectPosY, size_t rectPosX, size_t rectWidth, size_t rectHeight)
	{
		int A = 0, B = 0, C = 0, D = 0;
		if (rectPosX > 0 && rectPosY > 0)
			A = summedAreaTableI[rectPosX - 1][rectPosY - 1];
		if (rectPosY > 0)
			B = summedAreaTableI[rectPosX + rectWidth - 1][rectPosY - 1];
		if (rectPosX > 0)
			C = summedAreaTableI[rectPosX - 1][rectPosY + rectHeight - 1];

		D = summedAreaTableI[rectPosX + rectWidth - 1][rectPosY + rectHeight - 1];

		size_t sum = A - B - C + D;

		return sum;
	}

	Image
	kernel_conv(const Kernel& kernel, const Image& image)
	{

		vector<vector<uint32_t>> summedAreaTableI;
		summedAreaTableI.resize(image.height);
		for (size_t i = 0; i < image.height; ++i)
			summedAreaTableI[i].resize(image.width);

		calculate_summed_area_table(summedAreaTableI, image);


		uint32_t offset = kernel.width / 2;

		Image self = image_new(image.width, image.height, image.channels);

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height - 50; ++i)
				for (size_t j = 0; j < image.width - 50; ++j){
					float value = calculate_mean(summedAreaTableI, i, j, kernel.height, kernel.width);
					self(i, j, k) = uint8_t(value / (kernel.width * kernel.height));
				}

		return self;
	}
}
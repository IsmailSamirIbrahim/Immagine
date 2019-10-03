#include "Immagine/Kernel.h"

#include <memory>

using namespace std;

#define M_PI	3.14159265358979323846  

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

	Kernel
	kernel_box_gen(size_t width, size_t height)
	{
		Kernel self = kernel_new(width, height);

		float val = (1.0f / (width * height));
		size_t size = width * height;
		for (size_t i = 0; i < size; ++i)
			self.data[i] = val;

		return self;
	}

	Kernel
	kernel_gaussian_gen(size_t width, size_t height, float standard_deviation)
	{
		Kernel self = kernel_new(width, height);

		int offset = width / 2;
		float s = standard_deviation * standard_deviation;
		float r;
		float sum = 0.0;   // Initialization of sum for normalization
		for (int x = -1 * offset; x <= offset; x++) // Loop to generate WxH kernel
		{
			for (int y = -1 * offset; y <= offset; y++)
			{
				r = sqrt(x*x + y * y);
				self(x + offset, y + offset) = (exp(-(r*r) / s)) / (M_PI * s);
				sum += self(x + offset, y + offset);
			}
		}

		// Loop to normalize the kernel
		for (int i = 0; i < self.height; ++i)
			for (int j = 0; j < self.width; ++j)
				self(i, j) /= sum;

		return self;
	}

}
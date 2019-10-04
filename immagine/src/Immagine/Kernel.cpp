#include "Immagine/Kernel.h"

#include <memory>
#include <cmath>

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

	int*
	kernel_gaussian_gen(float sigma, size_t n)
	{
		float wIdeal = sqrt((12 * sigma*sigma / n) + 1);  // Ideal averaging filter width 
		size_t wl = floor(wIdeal);
		if (wl % 2 == 0)
			wl--;

		size_t wu = wl + 2;

		float mIdeal = (12 * sigma*sigma - n * wl*wl - 4 * n*wl - 3 * n) / (-4 * wl - 4);
		size_t m = round(mIdeal);
		
		int* sizes = (int*)::malloc(n * sizeof(int));
		for (size_t i = 0; i < n; i++)
			sizes[i] = (i < m ? wl : wu);

		return sizes;
	}

}
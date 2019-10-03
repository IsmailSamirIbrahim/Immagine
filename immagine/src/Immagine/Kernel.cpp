#include "Immagine/Kernel.h"

#include <memory>

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

}
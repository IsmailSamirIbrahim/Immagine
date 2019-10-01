#include "Immagine/Kernel.h"
#include <memory>

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
		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height; ++i)
				for (size_t j = 0; j < image.width - kernel.width; ++j) {
					float val = 0.0f;
					for (size_t x = 0; x < kernel.width; ++x)
						val += image(i, j + x, k) * kernel.data[x];
					self(i, j + offset, k) = uint8_t(val);
				}
	}

	inline static void
	kernel_conv_vertical(const Kernel& kernel, const Image& image, Image& self)
	{
		uint32_t offset = kernel.height / 2;
		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height - kernel.height; ++i)
				for (size_t j = 0; j < image.width; ++j) {
					float val = 0.0f;
					for (size_t x = 0; x < kernel.height; ++x)
						val += image(i + x, j, k) * kernel.data[x];
					self(i + offset, j, k) = uint8_t(val);
				}
	}

	Image
	kernel_conv(const Kernel& kernel, const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		Kernel kernelh = kernel_new(51, 1);
		float val = (1.0f / (kernelh.width * kernelh.height));
		for (int i = 0; i < kernelh.width * kernelh.height; ++i)
			kernelh.data[i] = val;
		kernel_conv_horizental(kernelh, image, self);
		
		Kernel kernelv = kernel_new(1, 51);
		val = (1.0f / (kernelv.width * kernelv.height));
		for (int i = 0; i < kernelv.width * kernelv.height; ++i)
			kernelv.data[i] = val;
		kernel_conv_vertical(kernelv, image, self);

		return self;
	}
}
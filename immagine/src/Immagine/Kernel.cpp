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
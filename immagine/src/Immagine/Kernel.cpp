#include "Immagine/Kernel.h"
#include <memory>
#include <omp.h>

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

	Image
	kernel_conv(const Kernel& kernel, const Image& image)
	{
		uint32_t offset = kernel.width / 2;

		Image padded_image = image_pad(image, offset, offset, 0);

		Image self = image_new(image.width, image.height, image.channels);

		size_t nh = padded_image.height - (kernel.height / 2);
		size_t nw = padded_image.width - (kernel.width / 2);

#pragma omp parallel for
		for (int8_t k = 0; k < image.channels; ++k){
			for (size_t i = (kernel.height / 2); i < nh; ++i){
				for (size_t j = (kernel.width / 2); j < nw; ++j){
					float val = 0.0f;
					for (size_t y = 0; y < kernel.height; ++y){
						for (size_t x = 0; x < kernel.width; ++x){
							val += kernel(y, x) * padded_image(i + y - kernel.height / 2, j + x - kernel.width / 2, k);
						}
					}
					self(i - offset, j - offset, k) = uint8_t(val);
				}
			}
		}

		image_free(padded_image);

		return self;
	}
}
#include "Immagine/Convolution.h"
#include "Immagine/Kernel.h"
#include "Immagine/Utilities.h"

#include <vector>

using namespace std;

typedef vector<vector<vector<uint32_t>>> vec3ui;
typedef vector<vector<uint32_t>> vec2ui;
typedef vector<uint32_t> vec1ui;

namespace immagine
{
	// Helper Functions
	inline static void
	_image_filter_horizental(const Kernel& kernel, const Image& image, Image& self)
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
	_image_filter_vertical(const Kernel& kernel, const Image& image, Image& self)
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


	// API
	Image
	image_box_filter(const Image& image, size_t kernel_width, size_t kernel_height)
	{
		Image self = image_new(image.width, image.height, image.channels);

		Kernel kernel = kernel_box_gen(kernel_width, kernel_height);

		vec3ui summed_table(image.height, vec2ui(image.width, vec1ui(image.channels)));
		calculate_summed_area(image, summed_table);

		size_t width_offset = kernel.width / 2;
		size_t height_offset = kernel.height / 2;

		for (int8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height - kernel.height; ++i)
				for (size_t j = 0; j < image.width - kernel.width; ++j)
					self(i + height_offset, j + width_offset, k) = calculate_mean(i, j, k, kernel.height, kernel.width, summed_table);

		kernel_free(kernel);

		return self;
	}

}
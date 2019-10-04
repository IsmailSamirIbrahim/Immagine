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
	Image
	image_filter_horizental(const Image& image, size_t kernel_width)
	{
		Image self = image_new(image.width, image.height, image.channels);

		Kernel kernel = kernel_box_gen(kernel_width, 1);

		size_t offset = kernel_width / 2;
		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height; ++i)
				for (size_t j = 0; j < image.width - kernel_width + 1; ++j) {
					float val = 0.0f;
					for (size_t x = 0; x < kernel_width; ++x)
						val += image(i, j + x, k) * kernel.data[x];
					self(i, j + offset, k) = uint8_t(val);
				}

		//handle edges  left/right
		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height; ++i)
				for (size_t j = offset; j > 0; --j) {
					float val = 0.0f;
					for (size_t x = 0; x < kernel_width; ++x)
						val += image(i, j + x, k) * kernel.data[x];
					self(i, j - 1, k) = uint8_t(val);
				}

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height; ++i)
				for (size_t j = image.width - kernel_width - 1; j < image.width - 1; ++j) {
					float val = 0.0f;
					for (size_t x = 0; x < kernel_width; ++x)
						val += image(i, j - x, k) * kernel.data[x];
					self(i, j + 1, k) = uint8_t(val);
				}

		return self;
	}

	Image
	image_filter_vertical(const Image& image, size_t kernel_height)
	{
		Image self = image_new(image.width, image.height, image.channels);

		Kernel kernel = kernel_box_gen(1, kernel_height);

		size_t offset = kernel_height / 2;
		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height - kernel_height + 1; ++i)
				for (size_t j = 0; j < image.width; ++j) {
					float val = 0.0f;
					for (size_t x = 0; x < kernel_height; ++x)
						val += image(i + x, j, k) * kernel.data[x];
					self(i + offset, j, k) = uint8_t(val);
				}

		return self;
	}


	// API
	Image
	image_box_filter(const Image& image, size_t kernel_width, size_t kernel_height)
	{
		Image self = image_new(image.width, image.height, image.channels);

		Image padded_image = image_pad(image, kernel_width / 2, kernel_height / 2, 0);

		vec3ui summed_table(padded_image.height, vec2ui(padded_image.width, vec1ui(padded_image.channels)));
		calculate_summed_area(padded_image, summed_table);

		Kernel kernel = kernel_box_gen(kernel_width, kernel_height);

		for (uint8_t k = 0; k < padded_image.channels; ++k)
			for (size_t i = 0; i < padded_image.height - kernel_height + 1; ++i)
				for (size_t j = 0; j < padded_image.width - kernel_width + 1; ++j)
					self(i, j, k) = calculate_mean(i, j, k, kernel.height, kernel.width, summed_table);

		kernel_free(kernel);
		image_free(padded_image);

		return self;
	}

}
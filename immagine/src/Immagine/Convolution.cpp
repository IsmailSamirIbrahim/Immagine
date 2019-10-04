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
	inline static Image
	_image_box_filter(const Image& image, float standard_deviation)
	{
		int* sizes = kernel_gaussian_gen(standard_deviation, 3);

		Image imgh1 = image_horizental_filter(image, sizes[0]);
		Image imgv1 = image_vertical_filter(imgh1, sizes[0]);

		Image imgh2 = image_horizental_filter(imgv1, sizes[1]);
		Image imgv2 = image_vertical_filter(imgh2, sizes[1]);

		Image imgh3 = image_horizental_filter(imgv2, sizes[2]);
		Image self = image_vertical_filter(imgh3, sizes[2]);

		image_free(imgh1);
		image_free(imgh2);
		image_free(imgh3);
		image_free(imgv1);
		image_free(imgv2);
		::free(sizes);

		return self;
	}

	// API
	Image
	image_horizental_filter(const Image& image, size_t kernel_width)
	{
		Image self = image_new(image.width, image.height, image.channels);

		int32_t offset = kernel_width / 2;
		Image p_image = image_pad(image, offset, 0, image(0, 0));

		Kernel kernel = kernel_box_gen(kernel_width, 1);

		size_t nw = p_image.width - (kernel_width / 2);

		for (uint8_t k = 0; k < p_image.channels; ++k)
			for (size_t i = 0; i < p_image.height; ++i)
				for (size_t j = offset; j < nw; ++j) {
					float val = 0.0f;
					for (size_t x = 0; x < kernel_width; ++x)
						val += p_image(i, j + x - offset, k) * kernel.data[x];
					self(i, j - offset, k) = uint8_t(val);
				}

		image_free(p_image);
		kernel_free(kernel);

		return self;
	}

	Image
	image_vertical_filter(const Image& image, size_t kernel_height)
	{
		Image self = image_new(image.width, image.height, image.channels);

		int32_t offset = kernel_height / 2;
		Image p_image = image_pad(image, 0, offset, image(0, 0));

		Kernel kernel = kernel_box_gen(1, kernel_height);

		size_t nh = p_image.height - (kernel_height / 2);

		for (uint8_t k = 0; k < p_image.channels; ++k)
			for (size_t i = offset; i < nh; ++i)
				for (size_t j = 0; j < p_image.width; ++j) {
					float val = 0.0f;
					for (size_t x = 0; x < kernel_height; ++x)
						val += p_image(i + x - offset, j, k) * kernel.data[x];
					self(i - offset, j, k) = uint8_t(val);
				}

		image_free(p_image);
		kernel_free(kernel);

		return self;
	}

	Image
	image_box_filter(const Image& image, size_t kernel_width, size_t kernel_height)
	{
		Image self = image_new(image.width, image.height, image.channels);

		Image padded_image = image_pad(image, kernel_width / 2, kernel_height / 2, image(0, 0));

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

	Image
	image_gaussian_filter(const Image& image, float standard_deviation)
	{
		return _image_box_filter(image, standard_deviation);
	}
}
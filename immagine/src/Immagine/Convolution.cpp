#include "Immagine/Convolution.h"
#include "Immagine/Kernel.h"
#include "Immagine/Utilities.h"

#include <vector>
#include <omp.h>

using namespace std;

typedef vector<vector<vector<uint32_t>>> vec3ui;
typedef vector<vector<uint32_t>> vec2ui;
typedef vector<uint32_t> vec1ui;

namespace immagine
{
	// Helper Functions
	inline static void
	_image_handle_border(Image& self, size_t kernel_width, size_t kernel_height)
	{
		//handle left border
		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = 0; i < self.height; ++i)
				for (size_t j = 0; j < kernel_width / 2; ++j)
					self(i, j, k) = self(i, kernel_width / 2, k);

		//handle right border
		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = 0; i < self.height; ++i)
				for (size_t j = self.width - kernel_width / 2 - 1; j < self.width ; ++j)
					self(i, j, k) = self(i, self.width - kernel_width / 2 - 2, k);

		//handle upper border
		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = 0; i < kernel_height / 2; ++i)
				for (size_t j = 0; j < self.width; ++j)
					self(i, j, k) = self(kernel_height / 2, j, k);

		//handle bottom border
		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = self.height - kernel_height / 2 - 1; i < self.height; ++i)
				for (size_t j = 0; j < self.width; ++j)
					self(i, j, k) = self(self.height - kernel_height / 2 - 2, j, k);
	}


	// API
	Image
	image_horizental_filter(const Image& image, size_t kernel_width)
	{
		Image self = image_new(image.width, image.height, image.channels);

		Kernel kernel = kernel_box_gen(kernel_width, 1);

		size_t nw = image.width - kernel_width;

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height; ++i)
				for (size_t j = 0; j < nw; ++j) {
					float val = 0.0f;
					for (size_t x = 0; x < kernel_width; ++x)
						val += image(i, j + x, k) * kernel.data[x];
					self(i, j + kernel_width / 2, k) = uint8_t(val);
				}
		kernel_free(kernel);

		return self;
	}

	Image
	image_vertical_filter(const Image& image, size_t kernel_height)
	{
		Image self = image_new(image.width, image.height, image.channels);

		Kernel kernel = kernel_box_gen(1, kernel_height);

		size_t nh = image.height - kernel_height;

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < nh; ++i)
				for (size_t j = 0; j < image.width; ++j) {
					float val = 0.0f;
					for (size_t x = 0; x < kernel_height; ++x)
						val += image(i + x, j, k) * kernel.data[x];
					self(i + kernel_height / 2, j, k) = uint8_t(val);
				}

		kernel_free(kernel);

		return self;
	}

	Image
	image_box_filter(const Image& image, size_t kernel_width, size_t kernel_height)
	{
		Image h_image = image_horizental_filter(image, kernel_width);
		Image self = image_vertical_filter(h_image, kernel_height);

		_image_handle_border(self, kernel_width, kernel_height);

		image_free(h_image);

		return self;
	}

	Image
	image_gaussian_filter(const Image& image, float standard_deviation)
	{
		int* sizes = kernel_gaussian_gen(standard_deviation, 3);

		Image img1 = image_box_filter(image, sizes[0], sizes[0]);
		Image img2 = image_box_filter(img1, sizes[1], sizes[1]);
		Image self = image_box_filter(img2, sizes[2], sizes[2]);

		image_free(img1);
		image_free(img2);
		::free(sizes);

		return self;
	}

	Image
	image_median_filter(const Image& image, size_t kernel_width, size_t kernel_height)
	{
		Image self = image_new(image.width, image.height, image.channels);

		size_t middle = (kernel_width * kernel_height) / 2 + 1;

		size_t nh = (image.height % (kernel_height / 2)) == 0 ? (image.height - kernel_height + 1) : (image.height - kernel_height);
		size_t nw = (image.width % (kernel_width / 2)) == 0 ? (image.width - kernel_width + 1) : (image.width - kernel_width);

#pragma omp parallel for
		for (int8_t k = 0; k < image.channels; ++k) {
			map<uint8_t, size_t> hist;
			for (size_t i = 0; i < nh; ++i) {
				window_reset(hist, image, kernel_width, kernel_height, i, k);
				for (size_t j = 0; j < nw; ++j) {
					for (size_t r = 0; r < kernel_height; ++r)
					{
						hist_remove(hist, image(i + r, j, k));
						hist_add(hist, image(i + r, j + kernel_width, k));
					}
					self(i + kernel_height / 2, j + kernel_width / 2, k) = median_get(hist, middle);
				}
			}
		}

		return self;
	}

}
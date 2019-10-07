#include "Immagine/Convolution.h"
#include "Immagine/Kernel.h"
#include "Immagine/Utilities.h"

#include <vector>
#include <map>
#include <omp.h>

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

	inline static uint8_t
	median_get(const map<uint8_t, size_t>& hist, size_t middle)
	{
		map<uint8_t, size_t>::const_iterator it = hist.begin();
		size_t sum = 0;
		for (; it != hist.end(); ++it)
		{
			sum += it->second;
			if (sum >= middle)
				break;
		}
		return it->first;
	}

	inline static void
	hist_add(map<uint8_t, size_t>& hist, uint8_t val)
	{
		++hist[val];
	}

	inline static void
	hist_remove(map<uint8_t, size_t>& hist, uint8_t val)
	{
		map<uint8_t, size_t>::iterator it = hist.find(val);
		if (it->second >= 1)
			--it->second;
	}

	inline static void
	window_reset(map<uint8_t, size_t>& hist, const Image& image, size_t kernel_width, size_t kernel_height, size_t i, size_t k)
	{
		hist.clear();
		for (size_t r = 0; r < kernel_height; ++r)
			for (size_t c = 0; c < kernel_width; ++c)
				hist_add(hist, image(r + i, c, k));
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

	Image
	image_gaussian_filter(const Image& image, float standard_deviation)
	{
		return _image_box_filter(image, standard_deviation);
	}
}
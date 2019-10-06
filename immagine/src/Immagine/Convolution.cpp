#include "Immagine/Convolution.h"
#include "Immagine/Kernel.h"
#include "Immagine/Utilities.h"

#include <vector>
#include <map>

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

		map<uint8_t, size_t> hist;
		size_t middle = (kernel_width * kernel_height) / 2 + 1;

		for (int8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height - kernel_height + 1; ++i) {
				window_reset(hist, image, kernel_width, kernel_height, i, k);
				for (size_t j = 0; j < image.width - kernel_width + 1; ++j) {
					for (size_t r = 0; r < kernel_height; ++r)
					{
						hist_remove(hist, image(i + r, j, k));
						hist_add(hist, image(i + r, j + kernel_width, k));
					}
					self(i + kernel_height / 2, j + kernel_width / 2, k) = median_get(hist, middle);
				}
			}

		uint32_t x = kernel_width / 2;
		uint32_t y = kernel_height / 2;
		uint32_t width = image.width - (2 * (kernel_width / 2));
		uint32_t height = image.height - (2 * (kernel_height / 2));

		Image croped = image_crop(self, Rectangle{ x, y, width, height });
		Image resized = image_resize(croped, image.width, image.height, INTERPOLATION_METHOD::NEAREST_NEIGHBOUR);

		image_free(self);
		image_free(croped);

		return resized;
	}

}
#pragma once

#include "Immagine/Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Immagine/Disjoint_Set.h"

#include "Immagine/Utilities.h"

#include <vector>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <list>

using namespace std;

namespace immagine
{
	// Helper Functions
	inline static size_t
		_image_red_idx(const Image& image)
	{
		size_t index = image.width * image.height * 0;
		return index;
	}

	inline static size_t
		_image_green_idx(const Image& image)
	{
		size_t index = image.width * image.height * 1;
		return index;
	}

	inline static size_t
		_image_blue_idx(const Image& image)
	{
		size_t index = image.width * image.height * 2;
		return index;
	}

	inline static size_t
		_image_alpha_idx(const Image& image)
	{
		size_t index = image.width * image.height * 3;
		return index;
	}

	inline static Image
		_image_data_fork(unsigned char* data, uint32_t width, uint32_t height, uint8_t channels)
	{
		Image self = image_new(width, height, channels);

		size_t j = 0;
		size_t size = width * height * channels;
		for (uint8_t c = 0; c < channels; ++c)
			for (size_t i = c; i < size; i += channels)
				self.data[j++] = data[i];

		return self;
	}

	inline static Image
		_image_data_join(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		size_t j = 0;
		size_t size = image.width * image.height * image.channels;
		for (uint8_t c = 0; c < image.channels; ++c)
			for (size_t i = c; i < size; i += image.channels)
				self.data[i] = image.data[j++];

		return self;
	}

	inline static Image
		_image_resize_nearest_neighbour(const Image& image, uint32_t width, uint32_t height)
	{
		Image self = image_new(width, height, image.channels);

		float width_ratio = (float)image.width / (float)width;
		float height_ratio = (float)image.height / (float)height;

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < height; ++i)
				for (size_t j = 0; j < width; ++j)
					self(i, j, k) = image(size_t(floor(i * height_ratio)), size_t(floor(j * width_ratio)), k);

		return self;
	}

	inline static Image
		_image_resize_bilinear(const Image& image, uint32_t width, uint32_t height)
	{
		Image self = image_new(width, height, image.channels);

		float width_ratio = (float)image.width / (float)width;
		float height_ratio = (float)image.height / (float)height;

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < height - 1; ++i)
				for (size_t j = 0; j < width - 1; ++j)
				{
					size_t src_i = (size_t)(i * height_ratio);
					size_t src_j = (size_t)(j * width_ratio);

					float x_dist = (width_ratio  * j) - src_j;
					float y_dist = (height_ratio * i) - src_i;

					uint8_t p0 = image(src_i, src_j, k);
					uint8_t p1 = (src_j + 1 < image.width) ? image(src_i, src_j + 1, k) : 0;
					uint8_t p2 = (src_i + 1 < image.height) ? image(src_i + 1, src_j, k) : 0;
					uint8_t p3 = ((src_i + 1 < image.height) && (src_j + 1 < image.width)) ? image(src_i + 1, src_j + 1, k) : 0;

					float lp0 = p0 * (1 - x_dist) + p1 * x_dist;
					float lp1 = p2 * (1 - x_dist) + p3 * x_dist;

					uint32_t color = (uint32_t)(lp0 * (1 - y_dist) + lp1 * y_dist);
					self(i, j, k) = (color < BLACK) ? BLACK : (color > WHITE) ? WHITE : color;
				}

		return self;
	}

	struct Label_Info {
		uint32_t left;
		uint32_t above;
		uint32_t count;
	};

	inline static Label_Info
		_image_neighbouring_labels(uint32_t*** vec, size_t i, size_t j)
	{
		Label_Info labels{};
		labels.count = 0;
		labels.above = 0;
		labels.left = 0;

		// Pixel is not on top edge of image
		if (i > 0)
		{
			//It's a labelled pixel
			if (vec[i - 1][j][0] > 0)
			{
				labels.above = vec[i - 1][j][0];
				labels.count++;
			}
		}

		// Pixel is not on left edge of image
		if (j > 0)
		{
			//It's a labelled pixel
			if (vec[i][j - 1][0] > 0)
			{
				labels.left = vec[i][j - 1][0];
				labels.count++;
			}
		}

		return labels;
	}

	inline static Image
		_image_from_array3d(uint32_t*** vec, size_t width, size_t height)
	{
		Image self = image_new(width, height, 1);

		for (size_t i = 0; i < height; ++i)
			for (size_t j = 0; j < width; ++j)
				self(i, j) = vec[i][j][0];

		return self;
	}


	// API
	Image
		image_new(uint32_t width, uint32_t height, uint8_t channels)
	{
		Image self{};

		self.width = width;
		self.height = height;
		self.channels = channels;
		self.data = (uint8_t*)::malloc(width * height * channels * sizeof(uint8_t));

		::memset(self.data, 255, width * height * channels);

		return self;
	}

	Image
		image_clone(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		::memcpy(self.data, image.data, image.width * image.height * image.channels * sizeof(uint8_t));

		return self;
	}

	Image
		image_from_ptr(const void* data, uint32_t width, uint32_t height, uint8_t channels)
	{
		Image self = image_new(width, height, channels);

		::memcpy(self.data, data, width * height * channels * sizeof(uint8_t));

		return self;
	}

	void
		image_free(Image& self)
	{
		if (self.data)
			::free(self.data);
		self.data = nullptr;
	}

	Image
		image_load(const char* file_path)
	{
		int width, height, channels;
		unsigned char* data = stbi_load(file_path, &width, &height, &channels, 0);

		assert(data != NULL && "Can't open the image");

		Image self = _image_data_fork(data, width, height, channels);

		STBI_FREE(data);

		return self;
	}

	bool
		image_save(const char* file_path, const Image& image, IMAGE_FORMAT format)
	{
		Image self = _image_data_join(image);

		bool result;
		switch (format)
		{
		case IMAGE_FORMAT::BMP:
			result = stbi_write_bmp(file_path, int(self.width), int(self.height), int(self.channels), self.data);
			break;

		case IMAGE_FORMAT::PNG:
			result = stbi_write_png(file_path, int(self.width), int(self.height), int(self.channels), self.data, 0);
			break;

		case IMAGE_FORMAT::JPEG:
			result = stbi_write_jpg(file_path, int(self.width), int(self.height), int(self.channels), self.data, 0);
			break;

		default:
			assert(false && "Unsupported image format\n");
			result = false;
			break;
		}

		image_free(self);

		return result;
	}

	Image
		image_red_channel(const Image& image)
	{
		assert(image.channels >= 3 && "image is grayscale with 8-bit depth");

		Image self = image_new(image.width, image.height, 1);

		size_t j = _image_red_idx(image);
		size_t size = image.width * image.height;

		for (size_t i = 0; i < size; ++i, ++j)
			self.data[i] = image.data[j];

		return self;
	}

	Image
		image_green_channel(const Image& image)
	{
		assert(image.channels >= 3 && "image is grayscale with 8-bit depth");

		Image self = image_new(image.width, image.height, 1);

		size_t j = _image_green_idx(image);
		size_t size = image.width * image.height;

		for (size_t i = 0; i < size; ++i, ++j)
			self.data[i] = image.data[j];

		return self;
	}

	Image
		image_blue_channel(const Image& image)
	{
		assert(image.channels >= 3 && "image dosen't have blue channel");

		Image self = image_new(image.width, image.height, 1);

		size_t j = _image_blue_idx(image);
		size_t size = image.width * image.height;

		for (size_t i = 0; i < size; ++i, ++j)
			self.data[i] = image.data[j];

		return self;
	}

	Image
		image_alpha_channel(const Image& image)
	{
		assert(image.channels == 4 && "image dosen't have an alpha channel");

		Image self = image_new(image.width, image.height, 1);

		size_t j = _image_alpha_idx(image);
		size_t size = image.width * image.height;

		for (size_t i = 0; i < size; ++i, ++j)
			self.data[i] = image.data[j];

		return self;
	}

	Image
		image_grayscale(const Image& image)
	{
		// Image is already grayscale image
		if (image.channels == 1) {
			Image self = image_clone(image);
			return self;
		}

		Image self = image_new(image.width, image.height, 1);

		size_t r = _image_red_idx(image);
		size_t g = _image_green_idx(image);
		size_t b = _image_blue_idx(image);
		size_t size = image.width * image.height;

		for (size_t i = 0; i < size; ++i, ++r, ++g, ++b)
			self.data[i] = uint8_t(float((image.data[r]) + (image.data[g]) + (image.data[b])) / 3.0f);

		return self;
	}

	Image
		image_flip_horizontally(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height; ++i)
				for (size_t j = 0; j < image.width; ++j)
					self(i, j, k) = image(i, image.width - 1 - j, k);

		return self;
	}

	Image
		image_flip_vertically(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height; ++i)
				for (size_t j = 0; j < image.width; ++j)
					self(i, j, k) = image(image.height - 1 - i, j, k);

		return self;
	}

	Image
		image_mirror(const Image& image)
	{
		return image_flip_horizontally(image);
	}

	Image
		image_rotate_right(const Image& image)
	{
		Image self = image_new(image.height, image.width, image.channels);

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height; ++i)
				for (size_t j = 0; j < image.width; ++j)
					self(j, image.height - 1 - i, k) = image(i, j, k);

		return self;
	}

	Image
		image_rotate_left(const Image& image)
	{
		Image self = image_new(image.height, image.width, image.channels);

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height; ++i)
				for (size_t j = 0; j < image.width; ++j)
					self(image.width - 1 - j, i, k) = image(i, j, k);

		return self;
	}

	Image
		image_rotate(const Image& image, float theta)
	{
		Image self = image_new(image.width, image.height, image.channels);

		float rads = theta * 3.1415926 / 180.0; // fixed constant PI
		float xcenter = (float)(image.width) / 2.0;   // use float here!
		float ycenter = (float)(image.height) / 2.0;

		for (uint8_t k = 0; k < image.channels; ++k)
			for (size_t i = 0; i < image.height; ++i)
				for (size_t j = 0; j < image.width; ++j)
				{
					int rorig = ycenter + ((float)(i)-ycenter)*cos(rads) - ((float)(j)-xcenter)*sin(-rads);
					int corig = xcenter + ((float)(i)-ycenter)*sin(-rads) + ((float)(j)-xcenter)*cos(rads);
					// now get the pixel value if you can
					uint8_t pixel = 0; // in case there is no original pixel
					if (rorig >= 0 && rorig < image.height && corig >= 0 && corig < image.width)
						pixel = image(rorig, corig, k);
					self(i, j, k) = pixel;
				}
		return self;
	}

	Image
		image_resize(const Image & image, uint32_t width, uint32_t height, INTERPOLATION_METHOD method)
	{
		switch (method)
		{
		case INTERPOLATION_METHOD::NEAREST_NEIGHBOUR:
			return _image_resize_nearest_neighbour(image, width, height);

		case INTERPOLATION_METHOD::BILINEAR:
			return _image_resize_bilinear(image, width, height);

		default:
			assert(false && "Unreachable state");
			return Image();
		}
	}

	Image
		image_crop(const Image & image, const Rectangle& rect)
	{
		Image self = image_new(rect.width, rect.height, image.channels);

		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = rect.y; i < rect.y + rect.height; ++i)
				for (size_t j = rect.x; j < rect.x + rect.width; ++j)
					self(i - rect.y, j - rect.x, k) = image(i, j, k);

		return self;
	}

	Image
		image_pad(const Image& image, uint32_t expand_w, uint32_t expand_h, uint8_t val)
	{
		Image self = image_new(image.width + 2 * expand_w, image.height + 2 * expand_h, image.channels);

		::memset(self.data, val, self.width * self.height * self.channels);

		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = expand_h; i < self.height - expand_h; ++i)
				for (size_t j = expand_w; j < self.width - expand_w; ++j)
					self(i, j, k) = image(i - expand_h, j - expand_w, k);

		return self;
	}

	Image
		image_binarize(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		size_t size = image.width * image.height * image.channels;
		size_t sum = 0;
		for (size_t i = 0; i < size; ++i)
			sum += image.data[i];

		uint8_t threshold = ((float)sum / (float)size + 0.5f);
		for (size_t i = 0; i < size; ++i)
			self.data[i] = (image.data[i] > threshold) ? WHITE : BLACK;

		return self;
	}

	Image
		image_connected_component(const Image & image)
	{
		uint32_t*** vec = array3d_new(image.width, image.height, image.channels);

		Disjoint_Set uf = disjoint_set_new(image.width * image.height);
		uint32_t current_label = 1;

		//1st Pass : label image and record label equivalences
		for (size_t i = 0; i < image.height; ++i) {
			for (size_t j = 0; j < image.width; ++j) {

				//background
				if (image(i, j) == 0) {
					vec[i][j][0] = 0;
					continue;
				}

				Label_Info labels = _image_neighbouring_labels(vec, i, j);

				//If no neighbouring foreground pixels, new label->use current_label
				if (labels.count == 0)
				{
					vec[i][j][0] = current_label;
					//record label in disjoint set
					++current_label;
				}
				else
				{
					uint32_t smallest_label;

					if (labels.above != 0 && labels.left == 0)
						smallest_label = labels.above;
					else if (labels.left != 0 && labels.above == 0)
						smallest_label = labels.left;
					else
						smallest_label = (labels.left < labels.above) ? labels.left : labels.above;

					vec[i][j][0] = smallest_label;

					//# More than one type of label in component
					if (labels.count > 1 && labels.left != labels.above)
					{
						//add equivalence class
						disjoint_set_union(uf, smallest_label, labels.left);
						disjoint_set_union(uf, smallest_label, labels.above);
					}
				}
			}
		}

		//2nd Pass: replace labels with their root labels
		for (size_t i = 0; i < image.height; ++i) {
			for (size_t j = 0; j < image.width; ++j) {

				if (vec[i][j][0] > 0)
				{
					uint8_t new_label = disjoint_set_find_root(uf, vec[i][j][0]);
					vec[i][j][0] = new_label;
				}
			}
		}

		Image self = _image_from_array3d(vec, image.width, image.height);

		disjoint_set_free(uf);
		array3d_free(vec, image.width, image.height, image.channels);

		return self;
	}

	int
		image_otsu_threshold(const Image & image)
	{
		// calculate histogram
		int histogram[256] = { 0 };
		for (int i = 0; i < image.height; ++i)
			for (int j = 0; j < image.width; ++j)
				++histogram[image(i, j, 0)];

		// calculate probability
		double p[256] = { 0 };
		for (int i = 0; i < 256; i++)
			p[i] = (double)histogram[i] / (double)image.width * image.height;

		double total_mean = 0;
		// calculate total mean for whole image
		for (int i = 0; i < 256; i++)
			total_mean += i * p[i];



		int optimalTreshold1 = 0, optimalTreshold2 = 0, optimalTreshold3 = 0;
		double maxBetweenVar = 0;
		double c = 0;

		double w0 = 0, m0 = 0, c0 = 0, p0 = 0;
		double w1 = 0, m1 = 0, c1 = 0, p1 = 0;
		double w2 = 0, m2 = 0, c2 = 0, p2 = 0;

		for (int tr1 = 0; tr1 < 256; tr1++)
		{
			p0 += p[tr1];
			w0 += (tr1 * p[tr1]);
			if (p0 != 0)
				m0 = w0 / p0;
			c0 = p0 * (m0 - total_mean) * (m0 - total_mean);

			c1 = 0, w1 = 0, m1 = 0, p1 = 0;
			for (int tr2 = tr1 + 1; tr2 < 256; tr2++)
			{
				p1 += p[tr2];
				w1 += (tr2 * p[tr2]);
				if (p1 != 0)
					m1 = w1 / p1;
				c1 = p1 * (m1 - total_mean) * (m1 - total_mean);

				c2 = 0, w2 = 0, m2 = 0, p2 = 0;
				for (int tr3 = tr2 + 1; tr3 < 256; tr3++)
				{
					p2 += p[tr3];
					w2 += (tr3 * p[tr3]);
					if (p2 != 0)
						m2 = w2 / p2;
					c2 = p2 * (m2 - total_mean) * (m2 - total_mean);

					double p3 = 1 - (p0 + p1 + p2);
					double w3 = total_mean - (w0 + w1 + w2);
					double m3 = w3 / p3;
					double c3 = p3 * (m3 - total_mean) * (m3 - total_mean);

					double c = c0 + c1 + c2 + c3;

					if (maxBetweenVar < c)
					{
						maxBetweenVar = c;
						optimalTreshold1 = tr1;
						optimalTreshold2 = tr2;
						optimalTreshold3 = tr3;
					}
				}
			}
		}
		return 0;
	}

	std::vector<std::vector<uint8_t>> clusters(5);

	inline static std::vector<uint8_t>
	_get_centroids(const Image& image, int k)
	{
		std::vector<uint8_t> centers;
		for (int i = 0; i < k; ++i)
		{
			int x = rand() % image.width;
			int y = rand() % image.height;
			centers.push_back(image(y, x));
		}
		return centers;
	}

	inline static void
	_argmindistance(uint8_t pixel, std::vector<uint8_t> centriods)
	{
		std::vector<float> distance;
		
		for (int c = 0; c < centriods.size(); ++c) {
			float dist = sqrt(pow((centriods[c] - pixel), 2));
			distance.push_back(dist);
		}

			int minindex = -1;
			float MIN = 256;
			for (int i = 0; i < distance.size(); ++i)
				if (distance[i] < MIN) {
					MIN = distance[i];
					minindex = i;
				}

			clusters[minindex].push_back(pixel);
	}
	
	inline static uint8_t
	_get_average(std::vector<uint8_t> cluster)
	{
		int sum = 0;
		for (int i = 0; i < cluster.size(); ++i)
			sum += cluster[i];

		uint8_t res = (sum / cluster.size());
		return res;
	}

	int
	image_kmeans(const Image& image)
	{
		int k = 5;
		auto centroids = _get_centroids(image, k);

		int l = 10;
		while (l > 0)
		{
			for(int i = 0; i < 5; ++i)
				clusters[i].clear();

			for (int x = 0; x < image.height; ++x)
				for (int y = 0; y < image.width; ++y)
					_argmindistance(image(x, y), centroids);

			int index = 0;
			for (const auto& cluster : clusters)
				if (cluster.size() != 0)
				{
					centroids[index] = _get_average(cluster);
					index += 1;
				}
			--l;
		}

		int min = *std::min_element(clusters[4].begin(), clusters[4].end());
		int max = *std::max_element(clusters[4].begin(), clusters[4].end());

		
		Image self = image_new(image.width, image.height, 1);

		size_t size = image.width * image.height * 1;
		
		for (size_t i = 0; i < size; ++i)
			if (image.data[i] >= min && image.data[i] < max)
				self.data[i] = WHITE;
			else
				self.data[i] = BLACK;

		image_save("D:/4.bmp", self, IMAGE_FORMAT::BMP);

		return 0;

	}

	short x[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	short y[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };

	vector<pair<size_t, size_t>>
	_get_neightbours(const Image& image, size_t seed_x, size_t seed_y,  uint8_t t)
	{
		vector<pair<size_t, size_t>> result;

		if (seed_x - 1 >= 0 && seed_y - 1 >= 0 && image(seed_x - 1, seed_y - 1) >= t)
			result.push_back(make_pair(seed_x - 1, seed_y - 1));
		if (seed_y - 1 >= 0 && image(seed_x, seed_y - 1) >= t)
			result.push_back(make_pair(seed_x, seed_y - 1));
		if (seed_x + 1 < image.width && seed_y - 1 >= 0 && image(seed_x + 1, seed_y - 1) >= t)
			result.push_back(make_pair(seed_x + 1, seed_y - 1));

		if (seed_x - 1 >= 0 && image(seed_x - 1, seed_y) >= t)
			result.push_back(make_pair(seed_x - 1, seed_y));
		if (seed_x + 1 < image.width && image(seed_x + 1, seed_y) >= t)
			result.push_back(make_pair(seed_x + 1, seed_y));

		if (seed_x - 1 >= 0 && seed_y + 1 < image.height && image(seed_x - 1, seed_y + 1) >= t)
			result.push_back(make_pair(seed_x - 1, seed_y + 1));
		if (seed_y + 1 < image.height && image(seed_x, seed_y + 1) >= t)
			result.push_back(make_pair(seed_x, seed_y + 1));
		if (seed_x + 1 < image.width && seed_y + 1 < image.height && image(seed_x + 1, seed_y + 1) >= t)
			result.push_back(make_pair(seed_x + 1, seed_y + 1));

		return result;
	}

	Image
	image_region_growing(const Image& image, size_t seed_x, size_t seed_y)
	{
		Image self = image_new(image.width, image.height, 1);
		::memset(self.data, 0, self.width * self.height);

		Image visited = image_new(image.width, image.height, 1);
		::memset(visited.data, 0, visited.width * visited.height);

		list<pair<size_t, size_t>> queue;

		queue.push_back(make_pair(seed_x, seed_y));
		visited(seed_x, seed_y) = 1;

		while (!queue.empty())
		{
			auto curr_seed = queue.front();
			queue.pop_front();
			self(curr_seed.first, curr_seed.second) = 255;

			auto list = _get_neightbours(image, curr_seed.first, curr_seed.second, 50);
			for (const auto& pair : list)
			{
				if (visited(pair.first, pair.second) == 0)
				{
					visited(pair.first, pair.second) = 1;
					queue.push_back(pair);
				}
			}
		}
		return self;
	}
}

#pragma once

#include "Immagine/Image.h"
#include "Immagine/Mask.h"

#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace immagine
{
	// Helper Functions
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


	// API
	Image
	image_new(uint32_t width, uint32_t height, uint8_t channels)
	{
		assert(width != 0 && height != 0 && channels != 0 && "Width, height and channels must be grater than zero");

		Image self{};

		self.width = width;
		self.height = height;
		self.channels = channels;
		self.data = (uint8_t*)::malloc(width * height * channels * sizeof(uint8_t));

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
		self.width = 0;
		self.height = 0;
		self.channels = 0;
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
	image_save(const char* file_path, const Image& image, IMAGE_FORMAT kind)
	{
		Image self = _image_data_join(image);

		bool result;
		switch (kind)
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
		assert(image.channels >= 3 && "image dosen't have red channel");

		Image self = image_new(image.width, image.height, 1);

		size_t size = image.width * image.height;
		size_t i = 0;
		size_t j = 0;
		while (size--)
			self.data[i++] = image.data[j++];

		return self;
	}

	Image
	image_green_channel(const Image& image)
	{
		assert(image.channels >= 3 && "image dosen't have green channel");

		Image self = image_new(image.width, image.height, 1);

		size_t size = image.width * image.height;
		size_t i = 0;
		size_t j = image.width * image.height;
		while (size--)
			self.data[i++] = image.data[j++];

		return self;
	}

	Image
	image_blue_channel(const Image& image)
	{
		assert(image.channels >= 3 && "image dosen't have blue channel");

		Image self = image_new(image.width, image.height, 1);

		size_t size = image.width * image.height;
		size_t i = 0;
		size_t j = image.width * image.height * 2;
		while (size--)
			self.data[i++] = image.data[j++];

		return self;
	}

	Image
	image_alpha_channel(const Image& image)
	{
		assert(image.channels == 4 && "image dosen't have alpha channel");

		Image self = image_new(image.width, image.height, 1);

		size_t size = image.width * image.height;
		size_t i = 0;
		size_t j = image.width * image.height * 3;
		while (size--)
			self.data[i++] = image.data[j++];

		return self;
	}

	Image
	image_gray_scale(const Image& image)
	{
		// Image is already gray scale image
		if (image.channels == 1) {
			Image self = image_clone(image);
			return self;
		}

		Image self = image_new(image.width, image.height, 1);

		size_t i = 0;
		size_t r = 0;
		size_t g = image.width * image.height;
		size_t b = image.width * image.height * 2;
		size_t size = image.width * image.height;

		while(size--)
			self.data[i++] = uint8_t(float((image.data[r++]) + (image.data[g++]) + (image.data[b++])) / 3.0f);

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
	_image_nearest_neighbour_algorithm(const Image& image, uint32_t width, uint32_t height)
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

	Image
	_image_bilinear_algorithm(const Image& image, uint32_t width, uint32_t height)
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
					self(i, j, k) = (color < BLACK) ? BLACK : (color > WHITE) ? WHITE: color;
				}

		return self;
	}

	Image
	_image_bicubic_algorithm(const Image& image, uint32_t width, uint32_t height)
	{
		return Image();
	}

	Image
	image_resize(const Image & image, uint32_t width, uint32_t height, SCALLING_ALGORITHM algorithm)
	{
		switch (algorithm)
		{
		case SCALLING_ALGORITHM::NEAREST_NEIGHBOUR:
			return _image_nearest_neighbour_algorithm(image, width, height);

		case SCALLING_ALGORITHM::BILINEAR:
			return _image_bilinear_algorithm(image, width, height);

		case SCALLING_ALGORITHM::BICUBIC:
			return _image_bicubic_algorithm(image, width, height);

		default:
			assert(false && "Unreachable state");
			return Image();
		}
	}

	Image
	image_crop(const Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		Image self = image_new(width, height, image.channels);

		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = y; i < y + height; ++i)
				for (size_t j = x; j < x + width; ++j)
					self(i - y, j - x, k) = image(i, j, k);

		return self;
	}

	Image
	image_blur(const Image& image, uint8_t size, IMAGE_FILTERS type)
	{
		Image self = image_new(image.width, image.height, image.channels);

		Mask mask = mask_generate(size, type);

		for (uint8_t k = 0; k < self.channels; ++k) {
			for (size_t i = 0; i < self.height; ++i) {
				for (size_t j = 0; j < self.width; ++j) {
					float value = 0.0f;
					for (uint8_t n = 0; n < size; ++n) {
						for (uint8_t m = 0; m < size; ++m) {
							size_t r = i + n;
							size_t c = j + m;

							r = r >= image.height ? image.height - 1 : r;
							c = c >= image.width ? image.width - 1 : c;

							value += float(image(r, c, k)) * mask(n, m);
						}
					}
					self(i, j, k) = uint8_t(value);
				}
			}
		}
		maske_free(mask);

		return self;
	}
}
#pragma once

#include "Immagine/Image.h"
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace immagine
{
	Image
	image_new(uint16_t width, uint16_t height, uint8_t channels)
	{
		assert(width != 0 && height != 0 && channels != 0 && "Width, height and channels must be grater than zero");

		Image self{};

		self.width = width;
		self.height = height;
		self.channels = channels;
		self.data = (Byte*)::malloc(width * height * channels * sizeof(Byte));
		::memset(self.data, 0, width * height * channels);

		return self;
	}

	Image
	image_clone(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		::memcpy(self.data, image.data, image.width * image.height * image.channels * sizeof(Byte));

		return self;
	}

	Image
	image_from_ptr(const void* data, uint16_t width, uint16_t height, uint8_t channels)
	{
		Image self = image_new(width, height, channels);

		::memcpy(self.data, data, width * height * channels * sizeof(Byte));

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
	
	void
	image_set_pixel(Image& image, uint16_t row, uint16_t column, Color color)
	{
		image(row, column, 0) = color.red;
		image(row, column, 1) = color.green;
		image(row, column, 2) = color.blue;
	}

	inline static void
	_image_data_parser(Byte* src, Byte* dst, uint16_t width, uint16_t height, uint8_t channels)
	{
		uint16_t j = 0;
		uint16_t size = width * height * channels;
		for (uint8_t c = 0; c < channels; ++c)
			for (uint16_t i = c; i < size; i += channels)
				dst[j++] = src[i];
	}

	Image
	image_load(const char* file_path)
	{
		int width, height, channels;
		Byte* data = stbi_load(file_path, &width, &height, &channels, 0);

		assert(data != NULL && "Can't open the image");

		Image self = image_new(width, height, channels);

		_image_data_parser(data, self.data, width, height, channels);

		STBI_FREE(data);

		return self;
	}

	inline static Image
	_image_data_parser_inv(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		uint16_t j = 0;
		uint16_t size = image.width * image.height * image.channels;
		for (uint8_t c = 0; c < image.channels; ++c)
			for (uint16_t i = c; i < size; i += image.channels)
				self.data[i] = image.data[j++];

		return self;
	}

	bool
	image_save(const char* file_path, const Image& image, IMAGE_FORMAT kind)
	{
		Image self = _image_data_parser_inv(image);

		bool result;
		switch (kind)
		{
		case BMP:
			result = stbi_write_bmp(file_path, int(self.width), int(self.height), int(self.channels), self.data);
			break;

		case PNG:
			result = stbi_write_png(file_path, int(self.width), int(self.height), int(self.channels), self.data, 0);
			break;

		case JPEG:
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

		uint16_t size = image.width * image.height;
		uint16_t i = 0;
		uint16_t j = 0;
		while (size--)
			self.data[i++] = image.data[j++];

		return self;
	}

	Image
	image_green_channel(const Image& image)
	{
		assert(image.channels >= 3 && "image dosen't have green channel");

		Image self = image_new(image.width, image.height, 1);

		uint16_t size = image.width * image.height;
		uint16_t i = 0;
		uint16_t j = image.width * image.height;
		while (size--)
			self.data[i++] = image.data[j++];

		return self;
	}

	Image
	image_blue_channel(const Image& image)
	{
		assert(image.channels >= 3 && "image dosen't have blue channel");

		Image self = image_new(image.width, image.height, 1);

		uint16_t size = image.width * image.height;
		uint16_t i = 0;
		uint16_t j = image.width * image.height * 2;
		while (size--)
			self.data[i++] = image.data[j++];

		return self;
	}

	Image
	image_alpha_channel(const Image& image)
	{
		assert(image.channels == 4 && "image dosen't have alpha channel");

		Image self = image_new(image.width, image.height, 1);

		uint16_t size = image.width * image.height;
		uint16_t i = 0;
		uint16_t j = image.width * image.height * 3;
		while (size--)
			self.data[i++] = image.data[j++];

		return self;
	}

	Image
	image_gray_scale(const Image& image)
	{
		assert(image.channels > 1 && "Image is already gray scale image");

		Image self = image_new(image.width, image.height, 1);

		uint16_t i = 0;
		uint16_t r = 0;
		uint16_t g = image.width * image.height;
		uint16_t b = image.width * image.height * 2;
		uint16_t size = image.width * image.height;

		while(size--)
			self.data[i++] = Byte(float((image.data[r++]) + (image.data[g++]) + (image.data[b++])) / 3.0f);

		return self;
	}

	Image
	image_binarize(const Image& image)
	{
		assert(image.channels == 1 && "Image must be gray level image\n");

		Image self = image_new(image.width, image.height, image.channels);

		uint16_t size = image.width * image.height * image.channels;
		uint16_t sum = 0;
		for (uint16_t i = 0; i < size; ++i)
			sum += image.data[i];

		uint8_t threshold = ((float)sum / (float)size + 0.5f);
		for (uint16_t i = 0; i < size; ++i)
			self.data[i] = (image.data[i] > threshold) ? WHITE : BLACK;

		return self;
	}

	Image
	image_flip_horizontally(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (uint8_t k = 0; k < image.channels; ++k)
			for (uint16_t i = 0; i < image.height; ++i)
				for (uint16_t j = 0; j < image.width; ++j)
					self(i, j, k) = image(i, image.width - 1 - j, k);

		return self;
	}

	Image
	image_flip_vertically(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (uint8_t k = 0; k < image.channels; ++k)
			for (uint16_t i = 0; i < image.height; ++i)
				for (uint16_t j = 0; j < image.width; ++j)
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
			for (uint16_t i = 0; i < image.height; ++i)
				for (uint16_t j = 0; j < image.width; ++j)
					self(j, image.height - 1 - i, k) = image(i, j, k);

		return self;
	}

	Image
	image_rotate_left(const Image& image)
	{
		Image self = image_new(image.height, image.width, image.channels);

		for (uint8_t k = 0; k < image.channels; ++k)
			for (uint16_t i = 0; i < image.height; ++i)
				for (uint16_t j = 0; j < image.width; ++j)
					self(image.width - 1 - j, i, k) = image(i, j, k);

		return self;
	}

	Image
	_image_nearest_neighbour_algorithm(const Image& image, uint16_t width, uint16_t height)
	{
		Image self = image_new(width, height, image.channels);

		float width_ratio = (float)image.width / (float)width;
		float height_ratio = (float)image.height / (float)height;

		for (uint8_t k = 0; k < image.channels; ++k)
			for (uint16_t i = 0; i < height; ++i)
				for (uint16_t j = 0; j < width; ++j)
					self(i, j, k) = image(uint16_t(floor(i * height_ratio)), uint16_t(floor(j * width_ratio)), k);

		return self;
	}

	Image
	_image_bilinear_algorithm(const Image& image, uint16_t width, uint16_t height)
	{
		Image self = image_new(width, height, image.channels);

		float width_ratio = (float)image.width / (float)width;
		float height_ratio = (float)image.height / (float)height;

		for (uint8_t k = 0; k < image.channels; ++k)
			for (uint16_t i = 0; i < height - 1; ++i)
				for (uint16_t j = 0; j < width - 1; ++j)
				{
					uint16_t src_i = (uint16_t)(i * height_ratio);
					uint16_t src_j = (uint16_t)(j * width_ratio);

					float x_dist = (width_ratio  * j) - src_j;
					float y_dist = (height_ratio * i) - src_i;

					Byte p0 = image(src_i, src_j, k);
					Byte p1 = (src_j + 1 < image.width) ? image(src_i, src_j + 1, k) : 0;
					Byte p2 = (src_i + 1 < image.height) ? image(src_i + 1, src_j, k) : 0;
					Byte p3 = ((src_i + 1 < image.height) && (src_j + 1 < image.width)) ? image(src_i + 1, src_j + 1, k) : 0;

					float lp0 = p0 * (1 - x_dist) + p1 * x_dist;
					float lp1 = p2 * (1 - x_dist) + p3 * x_dist;

					int color = (int)(lp0 * (1 - y_dist) + lp1 * y_dist);
					self(i, j, k) = (color < BLACK) ? BLACK : (color > WHITE) ? WHITE: color;
				}

		return self;
	}

	Image
	_image_bicubic_algorithm(const Image& image, uint16_t width, uint16_t height)
	{
		return Image();
	}

	Image
	image_resize(const Image & image, uint16_t width, uint16_t height, SCALLING_ALGORITHM algorithm)
	{
		switch (algorithm)
		{
		case NEAREST_NEIGHBOUR:
			return _image_nearest_neighbour_algorithm(image, width, height);

		case BILINEAR:
			return _image_bilinear_algorithm(image, width, height);

		case BICUBIC:
			return _image_bicubic_algorithm(image, width, height);

		default:
			assert(false && "Unreachable state");
			return Image();
		}
	}

	Image
	image_crop(const Image & image, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
	{
		Image self = image_new(width, height, image.channels);

		for (uint8_t k = 0; k < self.channels; ++k)
			for (uint16_t i = y; i < y + height; ++i)
				for (uint16_t j = x; j < x + width; ++j)
					self(i - y, j - x, k) = image(i, j, k);

		return self;
	}

}
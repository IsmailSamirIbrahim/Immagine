#pragma once

#include "Immagine/Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace immagine
{
	Image
	image_new(uint32_t width, uint32_t height, uint8_t channels)
	{
		assert(width != 0 && height != 0 && channels != 0 && "Width, height and channels must be grater than zero");

		Image self{};

		self.width = width;
		self.height = height;
		self.channels = channels;
		self.data = (Byte*)::malloc(width * height * channels * sizeof(Byte));

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
	image_from_ptr(const void* data, uint32_t width, uint32_t height, uint8_t channels)
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
	
	inline static void
	_image_data_parser(Byte* src, Byte* dst, uint32_t width, uint32_t height, uint8_t channels)
	{
		size_t j = 0;
		size_t size = width * height * channels;
		for (uint8_t c = 0; c < channels; ++c)
			for (size_t i = c; i < size; i += channels)
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

		size_t j = 0;
		size_t size = image.width * image.height * image.channels;
		for (uint8_t c = 0; c < image.channels; ++c)
			for (size_t i = c; i < size; i += image.channels)
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
		case IMAGE_FORMAT::BMP:
			result = stbi_write_bmp(file_path, self.width, self.height, self.channels, self.data);
			break;

		case IMAGE_FORMAT::PNG:
			result = stbi_write_png(file_path, self.width, self.height, self.channels, self.data, 0);
			break;

		case IMAGE_FORMAT::JPEG:
			result = stbi_write_jpg(file_path, self.width, self.height, self.channels, self.data, 0);
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
		assert(image.channels > 1 && "Image is already gray scale image");

		Image self = image_new(image.width, image.height, 1);

		size_t i = 0;
		size_t r = 0;
		size_t g = image.width * image.height;
		size_t b = image.width * image.height * 2;
		size_t size = image.width * image.height;

		while(size--)
			self.data[i++] = (float)((image.data[r++]) + (image.data[g++]) + (image.data[b++])) / 3.0f;

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
}
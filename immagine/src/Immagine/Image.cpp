#pragma once

#include "Immagine/Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace immagine
{
	/*************************utilities********************************************************/
	static inline Image
	_image_rgb_parser(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		size_t rgb[3] = { 0, self.width * self.height, self.width * self.height * 2 };

		size_t size = self.width * self.height * self.channels;
		for (size_t i = 0; i < size; i += self.channels)
			for (uint8_t k = 0; k < self.channels; ++k)
				self.data[i + k] = image.data[rgb[k]++];

		return self;
	}

	static inline Image
	_image_rgb_parser(uint32_t width, uint32_t height, uint8_t channels, Byte* data)
	{
		Image self = image_new(width, height, channels);

		size_t rgb[3] = { 0, width * height, width * height * 2 };

		size_t size = width * height * channels;
		for (size_t i = 0; i < size; i += channels)
			for (uint8_t k = 0; k < channels; ++k)
				self.data[rgb[k]++] = data[i + k];

		return self;
	}

	static inline Image
	_image_rgba_parser(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		size_t rgb[4] = { 0, self.width * self.height, self.width * self.height * 2, self.width * self.height * 3 };

		size_t size = self.width * self.height * self.channels;
		for (size_t i = 0; i < size; i += self.channels)
			for (uint8_t k = 0; k < self.channels; ++k)
				self.data[i + k] = image.data[rgb[k]++];

		return self;
	}

	static inline Image
	_image_rgba_parser(uint32_t width, uint32_t height, uint8_t channels, Byte* data)
	{
		Image self = image_new(width, height, channels);

		size_t rgb[4] = { 0, width * height, width * height * 2, width * height * 3 };

		size_t size = width * height * channels;
		for (size_t i = 0; i < size; i += channels)
			for (uint8_t k = 0; k < channels; ++k)
				self.data[rgb[k]++] = data[i + k];

		return self;
	}

	static inline Image
	_image_data_parser(const Image& image)
	{
		switch (image.channels)
		{
		case 1:
			return image;

		case 3:
			return _image_rgb_parser(image);

		case 4:
			return _image_rgba_parser(image);

		default:
			assert(false && "Unsupported image format/n");
			break;
		}
	}

	static inline Image
	_image_data_parser(uint32_t width, uint32_t height, uint8_t channels, Byte* data)
	{
		switch (channels)
		{
		case 1:
			return image_from_buffer(width, height, channels, data);

		case 3:
			return _image_rgb_parser(width, height, channels, data);

		case 4:
			return _image_rgba_parser(width, height, channels, data);

		default:
			assert(false && "Unsupported image format/n");
			break;
		}
	}
	
	static inline IMAGE_KIND
	_image_kind(uint8_t channels)
	{
		switch (channels)
		{
		case 1:
			return IMAGE_KIND::GRAY_SCALE;

		case 3:
			return IMAGE_KIND::RGB;

		case 4:
			return IMAGE_KIND::RGBA;

		default:
			assert(false && "Unsupport image format");
			return IMAGE_KIND::NONE;
		}
	}
	/******************************************************************************************/

	Image
	image_new(uint32_t width, uint32_t height, uint8_t channels)
	{
		assert(width != 0 && height != 0 && channels != 0 && "Width, hieght and channels must be grater than zero");

		Image self{};

		self.width = width;
		self.height = height;
		self.channels = channels;
		self.kind = _image_kind(channels);
		self.data =  (Byte*)::malloc(width * height * channels);

		return self;
	}

	void
	image_free(Image& self)
	{
		self.width = 0;
		self.height = 0;
		self.channels = 0;
		self.kind = IMAGE_KIND::NONE;
		if (self.data)
			::free(self.data);
		self.data = nullptr;
	}
	
	Image
	image_load(const char* file_path)
	{
		int width, height, channels;
		Byte* data = stbi_load(file_path, &width, &height, &channels, 0);

		assert(data != NULL && "Can't open the image");

		Image self = _image_data_parser(width, height, channels, data);
		self.kind = _image_kind(self.channels);

		STBI_FREE(data);

		return self;
	}

	bool
	image_save(const char* file_path, const Image& image, IMAGE_FORMAT kind)
	{
		Image self = _image_data_parser(image);

		switch (kind)
		{
		case IMAGE_FORMAT::BMP:
			return stbi_write_bmp(file_path, self.width, self.height, self.channels, self.data);

		case IMAGE_FORMAT::PNG:
			return stbi_write_png(file_path, self.width, self.height, self.channels, self.data, 0);

		case IMAGE_FORMAT::JPEG:
			return stbi_write_jpg(file_path, self.width, self.height, self.channels, self.data, 0);

		default:
			assert(false && "Unsupported image format\n");
			return false;
		}
	}

	Image
	image_clone(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		::memcpy(self.data, image.data, image.width * image.height * image.channels);

		return self;
	}

	Image
	image_from_buffer(uint32_t width, uint32_t height, uint8_t channels, const Byte* data)
	{
		Image self{};

		self.width = width;
		self.height = height;
		self.channels = channels;
		self.kind = _image_kind(channels);
		::memcpy(self.data, data, width* height* channels);

		return self;
	}

	Image
	image_red_channel(const Image& image)
	{
		assert(image.kind == IMAGE_KIND::RGB || image.kind == IMAGE_KIND::RGBA && "Image is not rgb or rgba");

		Image self = image_new(image.width, image.height, image.channels);

		// copy values of red channel
		::memcpy(
			self.data,
			image.data,
			self.width * self.height
		);

		// set the values of green channel to be zero
		::memset(
			self.data + self.width * self.height,
			0,
			self.width * self.height
		);

		// set the values of blue channel to be zero
		::memset(
			self.data + self.width * self.height * 2,
			0,
			self.width * self.height
		);

		if (image.channels == 4)
		{
			// copy values of alpha channel
			::memcpy(
				self.data + self.width * self.height * 3,
				image.data + image.width * image.height * 3,
				self.width * self.height
			);
		}

		return self;
	}

	Image
	image_green_channel(const Image& image)
	{
		assert(image.kind == IMAGE_KIND::RGB || image.kind == IMAGE_KIND::RGBA && "Image is not rgb or rgba");

		Image self = image_new(image.width, image.height, image.channels);

		// set the values of red channel to be zero
		::memset(
			self.data,
			0,
			self.width * self.height
		);

		// copy values of green channel
		::memcpy(
			self.data + self.width * self.height,
			image.data + self.width * self.height,
			self.width * self.height
		);

		// set the values of blue channel to be zero
		::memset(
			self.data + self.width * self.height * 2,
			0,
			self.width * self.height
		);

		if (image.channels == 4)
		{
			// copy values of alpha channel
			::memcpy(
				self.data + self.width * self.height * 3,
				image.data + image.width * image.height * 3,
				self.width * self.height
			);
		}

		return self;
	}

	Image
	image_blue_channel(const Image& image)
	{
		assert(image.kind == IMAGE_KIND::RGB || image.kind == IMAGE_KIND::RGBA && "Image is not rgb or rgba");

		Image self = image_new(image.width, image.height, image.channels);

		// set the values of red channel to be zero
		::memset(
			self.data,
			0,
			self.width * self.height
		);

		// set the values of green channel to be zero
		::memset(
			self.data + self.width * self.height,
			0,
			self.width * self.height
		);

		// copy values of blue channel
		::memcpy(
			self.data + self.width * self.height * 2,
			image.data + self.width * self.height * 2,
			self.width * self.height
		);

		if (image.channels == 4)
		{
			// copy values of alpha channel
			::memcpy(
				self.data + self.width * self.height * 3,
				image.data + image.width * image.height * 3,
				self.width * self.height
			);
		}

		return self;
	}
}
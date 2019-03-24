#pragma once

#include "Immagine/Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace immagine
{
	Image
	image_new(uint32_t width, uint32_t height, uint32_t depth)
	{
		assert(depth != 0 && "Unsupported image format");

		Image self{};

		self.width = width;
		self.height = height;
		self.depth = depth;
		self.data =  (Byte*)::malloc(width * height * depth * sizeof(Byte));

		return self;
	}

	void
	image_free(Image& self)
	{
		self.width = 0;
		self.height = 0;
		self.depth = 0;
		if (self.data)
			::free(self.data);
		self.data = nullptr;
	}

	Image
	image_load(const char* file_path)
	{
		Image self{};
		self.data = stbi_load(file_path, (int*)&self.width, (int*)&self.height, (int*)&self.depth, 0);
		return self;
	}

	bool
	image_save(const char* file_path, const Image& image, IMAGE_KIND kind)
	{
		switch (kind)
		{
		case IMAGE_KIND::BMP:
			return stbi_write_bmp(file_path, image.width, image.height, image.depth, image.data);

		case IMAGE_KIND::PNG:
			return stbi_write_png(file_path, image.width, image.height, image.depth, image.data, 0);

		case IMAGE_KIND::JPEG:
			return stbi_write_jpg(file_path, image.width, image.height, image.depth, image.data, 0);

		default:
			assert(false && "The kind of image is wierd\n");
			return false;
		}
	}

	Image
	image_red_channel(const Image & image)
	{
		assert(image.depth == 3 || image.depth == 4 && "Image must be 3D image with red, green, blue and/or alpha channel");

		Image self = image_new(image.width, image.height, image.depth);

		size_t size = image.width * image.height * image.depth;
		for (size_t i = 0; i < size; i += image.depth)
		{
			self.data[i] = image.data[i];
			self.data[i + 1] = 0;
			self.data[i + 2] = 0;
		}

		return self;
	}

	Image
	image_green_channel(const Image & image)
	{
		assert(image.depth == 3 || image.depth == 4 && "Image must be 3D image with red, green, blue and/or alpha channel");

		Image self = image_new(image.width, image.height, image.depth);

		size_t size = image.width * image.height * image.depth;
		for (size_t i = 0; i < size; i += image.depth)
		{
			self.data[i] = 0;
			self.data[i + 1] = image.data[i];
			self.data[i + 2] = 0;
		}

		return self;
	}

	Image
	image_blue_channel(const Image & image)
	{
		assert(image.depth == 3 || image.depth == 4 && "Image must be 3D image with red, green, blue and/or alpha channel");

		Image self = image_new(image.width, image.height, image.depth);

		size_t size = image.width * image.height * image.depth;
		for (size_t i = 0; i < size; i += image.depth)
		{
			self.data[i] = 0;
			self.data[i + 1] = 0;
			self.data[i + 2] = image.data[i];
		}

		return self;
	}

	void
	image_histogram(const Image& image, float hist[])
	{
		size_t size = image.width * image.height * image.depth;
		for (size_t i = 0; i < size; ++i)
			hist[image.data[i]] += 1;

		for (size_t i = 0; i < 256; ++i)
			hist[i] = (float)hist[i] / (float)size;
	}

	void
	image_histogram_red_channel(const Image & image, float hist[])
	{
		assert(image.depth == 3 || image.depth == 4 && "Image must be 3D image with red, green, blue and/or alpha channel");

		size_t size = image.width * image.height * image.depth;
		for (size_t i = 0; i < size; i += image.depth)
			hist[image.data[i]] += 1;

		for (size_t i = 0; i < 256; ++i)
			hist[i] = (float)hist[i] / (float)size;
	}

	void
	image_histogram_green_channel(const Image & image, float hist[])
	{
		assert(image.depth == 3 || image.depth == 4 && "Image must be 3D image with red, green, blue and/or alpha channel");

		size_t size = image.width * image.height * image.depth;
		for (size_t i = 1; i < size; i += image.depth)
			hist[image.data[i]] += 1;

		for (size_t i = 0; i < 256; ++i)
			hist[i] = (float)hist[i] / (float)size;
	}

	void
	image_histogram_blue_channel(const Image & image, float hist[])
	{
		assert(image.depth == 3 || image.depth == 4 && "Image must be 3D image with red, green, blue and/or alpha channel");

		size_t size = image.width * image.height * image.depth;
		for (size_t i = 2; i < size; i += image.depth)
			hist[image.data[i]] += 1;

		for (size_t i = 0; i < 256; ++i)
			hist[i] = (float)hist[i] / (float)size;
	}

	Image
	image_histogram_equalization(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.depth);

		// calculate PMF(Probability Mass Function)
		float hist[256] = {};
		image_histogram(image, hist);

		// calculate CDF(Cumulative Distributive Function)
		for (size_t i = 1; i < 256; ++i)
			hist[i] += hist[i - 1];

		// calculate CDF according to gray levels
		for (size_t i = 1; i < 256; ++i)
			hist[i] = (255 * hist[i] + 0.5f);

		// mapping new values with according pixel
		size_t size = image.width * image.height * image.depth;
		for (size_t i = 0; i < size; ++i)
			self.data[i] = hist[image.data[i]];

		return self;
	}

	Image
	image_brightness(const Image & image, int16_t brightness)
	{
		Image self = image_new(image.width, image.height, image.depth);

		size_t size = image.width * image.height * image.depth;
		for (size_t i = 0; i < size; ++i)
		{
			int16_t tmp = image.data[i] + brightness;
			self.data[i] = (tmp < 0) ? 0 : (tmp > 255) ? 255 : tmp;
		}

		return self;
	}

	Image
	image_binarize(const Image & image, uint8_t threshold)
	{
		assert(image.depth == 1 && "Image must be gray scale image\n");

		Image self = image_new(image.width, image.height, image.depth);

		size_t size = image.width * image.height * image.depth;
		for (size_t i = 0; i < size; ++i)
			self.data[i] = (image.data[i] > threshold) ? 255 : 0;

		return self;
	}

	Image
	image_gray_scale(const Image & image)
	{
		Image self = image_new(image.width, image.height, 1);

		size_t size = image.width * image.height * 1;
		for (size_t i = 0, j = 0; i < size; ++i, j += image.depth)
		{
			// using The luminosity method
			unsigned char gray_scale = ((0.21 * image.data[j]) + (0.72 * image.data[j + 1]) + (0.07 * image.data[j + 2])) / 3;
			self.data[i] = gray_scale;
		}

		return self;
	}

	inline static Image
	_image_pad_2d(const Image& image, uint8_t expand, unsigned char value)
	{
		Image self = image_new(image.width + 2 * expand, image.height + 2 * expand, 1);

		::memset(self.data, value, self.width * self.height);

		for (int j = expand; j < self.height - expand; ++j)
			for (int i = expand; i < self.width - expand; ++i)
				self(i, j) = image(i - expand, j - expand);

		return self;
	}

	inline static Image
	_image_pad_3d(const Image& image, uint8_t expand, unsigned char value)
	{
		Image self = image_new(image.width + 2 * expand, image.height + 2 * expand, image.depth + 2 * expand);

		::memset(self.data, value, self.width * self.height * self.depth);

		for (int k = expand; k < self.depth - expand; ++k)
			for (int j = expand; j < self.height - expand; ++j)
				for (int i = expand; i < self.width - expand; ++i)
					self(i, j, k) = image(i - expand, j - expand, k - expand);

		return self;
	}

	Image
	image_padding(const Image& image, uint8_t expand, Byte value)
	{
		switch (image.depth)
		{
		case 1:
			return _image_pad_2d(image, expand, value);
		case 3:
		case 4:
			return _image_pad_3d(image, expand, value);
		default:
			assert(false && "Unsupported image format");
			return image;
		}
	}
}
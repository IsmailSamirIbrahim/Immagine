#include "Immagine/Point_Processing.h"

namespace immagine
{
	Image
	image_bright_increase(const Image& image, uint8_t value)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = 0; i < self.height; ++i)
				for (size_t j = 0; j < self.width; ++j)
					self(i, j, k) = (image(i, j, k) + value) > 255 ? 255 : image(i, j, k) + value;

		return self;
	}

	Image
	image_bright_decrease(const Image& image, uint8_t value)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = 0; i < self.height; ++i)
				for (size_t j = 0; j < self.width; ++j)
					self(i, j, k) = (image(i, j, k) - value) < 0 ? 0 : image(i, j, k) - value;

		return self;
	}

	Image
	image_negative(const Image& image)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (uint8_t k = 0; k < self.channels; ++k)
			for (size_t i = 0; i < self.height; ++i)
				for (size_t j = 0; j < self.width; ++j)
					self(i, j, k) = (255 - image(i, j, k)) < 0 ? 0 : (255 - image(i, j, k));

		return self;
	}

}
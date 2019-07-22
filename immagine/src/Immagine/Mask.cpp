#include "Immagine/Mask.h"
#include <memory>

namespace immagine
{
	Mask
	mask_new(uint8_t width, uint8_t height)
	{
		assert(width != 0 && height != 0 && "Width and height must be grater than zero");

		Mask self{};

		self.width = width;
		self.height = height;
		self.data = (float*)::malloc(width * height * sizeof(float));

		return self;
	}

	inline static Mask
	_mask_box_generate(uint8_t size)
	{
		Mask self = mask_new(size, size);

		float val = 1.0f / float(size * size);

		for (uint8_t i = 0; i < self.height; ++i)
			for (uint8_t j = 0; j < self.width; ++j)
				self(i, j) = val;

		return self;
	}

	Mask
	mask_generate(uint8_t size, IMAGE_FILTERS type)
	{
		switch (type)
		{
		case MEAN_FILTER:
		case BOX_FILTER:
		case AVG_FILTER:
			return _mask_box_generate(size);
		case WEIGHTED_AVG_FILTER:
			break;
		case GAUSSIAN_FILTER:
			break;
		default:
			break;
		}
	}

	void
	maske_free(Mask mask)
	{
		if (mask.data)
			::free(mask.data);
		mask.data = nullptr;
	}
}
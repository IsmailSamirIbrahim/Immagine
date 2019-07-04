#pragma once

#include "Immagine/Image.h"
#include "Immagine/Imagedef.h"

namespace immagine
{
	inline static int8_t
	node_is_leaf(const Image& image, Region region)
	{
		for (size_t i = region.y; i < region.height; ++i)
			for (size_t j = region.x; j < region.x + region.width; ++j)
				if (image(i, j, 0) != image(region.y, region.x, 0))
					return -1;

		return image(region.y, region.x, 0);
	}
}
#include "Immagine/Point_Processing.h"
#include "Immagine/Utilities.h"

#include <vector>

using namespace std;

typedef vector<vector<vector<uint32_t>>> vec3ui;
typedef vector<vector<uint32_t>> vec2ui;
typedef vector<uint32_t> vec1ui;

namespace immagine
{
	Image
	image_erode(const Image& image, const Kernel& SE)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (size_t i = 0; i < image.height - SE.height; ++i)
			for (size_t j = 0; j < image.width - SE.width; ++j)
			{
				uint32_t hits = 0;
				for (size_t r = 0; r < SE.height; ++r)
					for (size_t c = 0; c < SE.width; ++c)
						hits += image(i + r, j + c) & uint8_t(SE(r, c));
				self(i + SE.height / 2, j + SE.width / 2) = hits == 1 ? 0: 255;
			}
		return self;
	}

}
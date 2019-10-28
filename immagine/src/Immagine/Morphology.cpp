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
	image_erode(Image& image, const Kernel& SE)
	{
		Image self = image_new(image.width, image.height, image.channels);

		for (size_t k = 0; k < 20; ++k) {
			for (size_t i = 0; i < image.height - SE.height; ++i)
				for (size_t j = 0; j < image.width - SE.width; ++j)
				{
					uint32_t hits = 0;
					for (size_t r = 0; r < SE.height; ++r)
						for (size_t c = 0; c < SE.width; ++c)
							if (image(i + r, j + c) == uint8_t(SE(r, c)) && image(i + r, j + c) == 255)
								hits++;
					if (hits >= 4)
						self(i + SE.height / 2, j + SE.width / 2) = 255;
					else
						self(i + SE.height / 2, j + SE.width / 2) = 0;
				}
			image = image_clone(self);
		}
		return self;
	}

}
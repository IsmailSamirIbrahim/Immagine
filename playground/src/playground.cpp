#include <iostream>

#include "Immagine/Image.h"
#include "Immagine/Point_Processing.h"
#include "Immagine/Convolution.h"
#include "Immagine/Disjoint_Set.h"
#include "Immagine/Utilities.h"

#include <chrono>
#include <string>

#include <map>
#include <vector>
#include <algorithm>
#include <random>
#include <vld.h>

using namespace std::chrono;
using namespace immagine;
using namespace std;

inline static Image
color_image(const Image& image)
{
	Image self = image_new(image.width, image.height, 3);

	std::map<uint8_t, std::vector<std::pair<size_t, size_t>>> map;

	for (size_t i = 0; i < image.height; ++i)
		for (size_t j = 0; j < image.width; ++j)
			if(image(i, j) > 0)
				map[image(i, j)].push_back(make_pair(i, j));

	std::vector<uint8_t> color(3);
	color[0] = 255;
	color[1] = 150;
	color[2] = 50;
	for (auto it = map.begin(); it != map.end(); ++it) {
		for (auto pair : it->second) {
			self(pair.first, pair.second, 0) = color[0];
			self(pair.first, pair.second, 1) = color[1];
			self(pair.first, pair.second, 2) = color[2];
		}

		color[0] = uint8_t(std::rand() % 255); // red component of color
		color[1] = uint8_t(std::rand() % 255); // green component of color
		color[2] = uint8_t(std::rand() % 255); // blue component of color
	}
	return self;
}

int
main(int argc, char** argv)
{
	string file_path = string(IMAGE_DIR) + string("/images/big.jpg");
	Image image = image_load(file_path.c_str());
	
	auto start = high_resolution_clock::now();

	Image result = image_rotate(image, -45);
		
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  seconds\n", duration.count());


	string out_path = string(IMAGE_DIR) + string("/images/result.bmp");
	image_save(out_path.c_str(), result, IMAGE_FORMAT::BMP);

	image_free(image);
	image_free(result);
	return 0;
}

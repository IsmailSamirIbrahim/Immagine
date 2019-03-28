#include <iostream>

#include "Immagine/Image.h"

#include <chrono>

using namespace std::chrono;
using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	Image image = image_load("D:/alpha.png");

	Image r = image_red_channel(image);
	Image g = image_green_channel(image);
	Image b = image_blue_channel(image);

	auto start = high_resolution_clock::now();

	image_save("D:/red.png", r, IMAGE_FORMAT::PNG);
	image_save("D:/green.png", g, IMAGE_FORMAT::PNG);
	image_save("D:/blue.png", b, IMAGE_FORMAT::PNG);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << duration.count() << endl;

	/*Image green = image_green_channel(image);
	image_save("D:/green.png", green, IMAGE_FORMAT::PNG);

	Image blue = image_blue_channel(image);
	image_save("D:/blue.png", blue, IMAGE_FORMAT::PNG);*/

	return 0;
}

#include <iostream>

#include "Immagine/Image.h"

#include <chrono>

using namespace std::chrono;
using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	Image image = image_load("D:/gray.jpg");

	Image img1 = image_brightness(image, 60);
	Image img2 = image_brightness(image, -60);

	image_save("D:/bright.png", img1, IMAGE_FORMAT::PNG);
	image_save("D:/dark.png", img2, IMAGE_FORMAT::PNG);

	Image img3 = image_binarize(image);
	image_save("D:/binarized.png", img3, IMAGE_FORMAT::PNG);

	/*Image r = image_red_channel(;image);
	Image g = image_green_channel(image);
	Image b = image_blue_channel(image);

	image_save("D:/red.png", r, IMAGE_FORMAT::PNG);
	image_save("D:/green.png", g, IMAGE_FORMAT::PNG);
	image_save("D:/blue.png", b, IMAGE_FORMAT::PNG);
	*/

	/*Image green = image_green_channel(image);
	image_save("D:/green.png", green, IMAGE_FORMAT::PNG);

	Image blue = image_blue_channel(image);
	image_save("D:/blue.png", blue, IMAGE_FORMAT::PNG);*/

	return 0;
}

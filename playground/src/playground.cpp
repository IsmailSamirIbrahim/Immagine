#include <iostream>

#include "Immagine/Image.h"

using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	Image image = image_load("E:/1.jpg");

	Image red = image_red_channel(image);
	image_save("E:/red.png", red, IMAGE_FORMAT::PNG);

	Image green = image_green_channel(image);
	image_save("E:/green.png", green, IMAGE_FORMAT::PNG);

	Image blue = image_blue_channel(image);
	image_save("E:/blue.png", blue, IMAGE_FORMAT::PNG);

	return 0;
}

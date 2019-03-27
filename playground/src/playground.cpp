#include <iostream>

#include "Immagine/Image.h"

using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	Image image = image_load("E:/lena_gray.bmp");

	Image img = image_red_channel(image);

	image_save("E:/save2.png", img, IMAGE_FORMAT::PNG);

	return 0;
}

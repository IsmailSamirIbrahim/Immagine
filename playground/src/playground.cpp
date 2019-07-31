#include <iostream>

#include "Immagine/Image.h"

#include <chrono>

using namespace std::chrono;
using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	Image img = image_load("D:/very_big.jpg");
	
	auto start = high_resolution_clock::now();

	//type code here.
	Image image = image_gray_scale(img);

	Image rotate_left = image_rotate_left(image);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());

	image_save("D:/rotate.bmp", rotate_left, IMAGE_FORMAT::BMP);

	return 0;
}

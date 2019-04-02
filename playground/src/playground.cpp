#include <iostream>

#include "Immagine/Image.h"

#include <chrono>

using namespace std::chrono;
using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	//type code here.
	Image img = image_load("D:/gray.png");

	auto start = high_resolution_clock::now();
	Image rotate = image_rotate_right(img);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());

	image_save("D:/rotate_right.jpg", rotate, IMAGE_FORMAT::JPEG);


	return 0;
}

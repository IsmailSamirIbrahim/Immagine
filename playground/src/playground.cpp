#include <iostream>

#include "Immagine/Image.h"

#include <chrono>

using namespace std::chrono;
using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	//auto start = high_resolution_clock::now();

	//type code here.

	Image image = image_new(100, 100, 3);

	for (size_t i = 40; i < 60; ++i)
		image_set_pixel(image, 50, i, BLUE);
	
	image = image_flip_vertically(image);

	image_save("D:/test.bmp", image, BMP);

	//auto stop = high_resolution_clock::now();
	//auto duration = duration_cast<seconds>(stop - start);
	//printf("Time = %lld  millisecond\n", duration.count());

	return 0;
}

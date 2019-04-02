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
	Image img = image_load("D:/1.jpg");

	auto start = high_resolution_clock::now();
	Image rotate = image_resize(img, 500, 500);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());

	image_save("D:/resized.png", rotate, IMAGE_FORMAT::PNG);


	return 0;
}

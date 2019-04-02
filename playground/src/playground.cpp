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
	Image img = image_load("D:/rgb.jpg");

	auto start = high_resolution_clock::now();
	Image flip = image_mirror(img);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());

	image_save("D:/fliped.jpg", flip, IMAGE_FORMAT::JPEG);


	return 0;
}

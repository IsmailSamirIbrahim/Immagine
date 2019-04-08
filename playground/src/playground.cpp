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
	Image img = image_load("D:/big.jpg");

	auto start = high_resolution_clock::now();
	//Image rotate = image_flip_horizontally(img);
	Image image = image_resize(img, 250, 250);
	image_save("D:/fliped.png", image, IMAGE_FORMAT::PNG);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());



	return 0;
}

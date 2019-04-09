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
	Image image1 = image_resize(img, 500, 500, NEAREST_NEIGHBOUR);
	Image image2 = image_resize(img, 500, 500, BILINEAR);
	image_save("D:/nearest.png", image1, IMAGE_FORMAT::PNG);
	image_save("D:/bilinear.png", image2, IMAGE_FORMAT::PNG);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());



	return 0;
}

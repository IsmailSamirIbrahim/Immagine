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
	Image image = image_crop(img, 2500, 4300, 1000, 1000);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());

	image_save("D:/test.bmp", image, BMP);

	return 0;
}

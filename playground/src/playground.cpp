#include <iostream>

#include "Immagine/Image.h"

#include <chrono>

using namespace std::chrono;
using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	Image image = image_load("E:/1.bmp");
	
	auto start = high_resolution_clock::now();

	//type code here.
	
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());

	image_save("E:/blured.bmp", image, FORMAT::BMP);

	return 0;
}

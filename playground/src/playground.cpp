#include <iostream>

#include "Immagine/Image.h"
#include "Immagine/Point_Processing.h"
#include "Immagine/Convolution.h"

#include <chrono>

//#include <vld.h>

using namespace std::chrono;
using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	Image image = image_load("E:/a.png");
	
	auto start = high_resolution_clock::now();

	//type code here.
	
	Image img = image_filter_horizental(image, 3);
	Image img1 = image_filter_vertical(img, 3);
	
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("Time = %lld  milliseconds\n", duration.count());

	image_save("E:/convolved_image.bmp", img1, IMAGE_FORMAT::BMP);

	image_free(image);
	image_free(img);

	return 0;
}

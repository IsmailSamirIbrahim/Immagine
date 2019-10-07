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
	Image image = image_load("D:/ismail/images/10.png");
	
	auto start = high_resolution_clock::now();

	//type code here.
	
	Image img1 = image_median_filter(image, 91, 91);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  seconds\n", duration.count());

	image_save("D:/ismail/images/result.bmp", img1, IMAGE_FORMAT::BMP);

	image_free(image);
	image_free(img1);

	return 0;
}

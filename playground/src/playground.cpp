#include <iostream>

#include "Immagine/Image.h"
#include "Immagine/Point_Processing.h"

#include <chrono>

using namespace std::chrono;
using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	Image image = image_load("E:/1.BMP");
	
	auto start = high_resolution_clock::now();

	//type code here.
	
	Image img1 = image_bright_increase(image, 100);
	Image img2 = image_bright_decrease(image, 100);
	Image img3 = image_negative(image);


	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());

	image_save("E:/img+.bmp", img1, FORMAT::BMP);
	image_save("E:/img-.bmp", img2, FORMAT::BMP);
	image_save("E:/img_.bmp", img3, FORMAT::BMP);

	image_free(image);
	image_free(img1);
	image_free(img2);
	image_free(img3);

	return 0;
}

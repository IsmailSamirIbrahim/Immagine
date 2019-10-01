#include <iostream>

#include "Immagine/Image.h"
#include "Immagine/Point_Processing.h"
#include "Immagine/Kernel.h"

#include <chrono>

using namespace std::chrono;
using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	Image image = image_load("D:/Ismail/images/fruits.bmp");
	
	auto start = high_resolution_clock::now();

	//type code here.
	
	Kernel kernel = kernel_new(1, 51);
	float val = (1.0f / (kernel.width * kernel.height));
	for (int i = 0; i < kernel.width * kernel.height; ++i)
		kernel.data[i] = val;

	Image img1 = kernel_conv(kernel, image);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());

	image_save("D:/Ismail/images/convolved_image.bmp", img1, FORMAT::BMP);

	image_free(image);
	image_free(img1);

	return 0;
}

#include <iostream>

#include "Immagine/Image.h"
#include "Immagine/Point_Processing.h"
#include "Immagine/Kernel.h"

#include <chrono>

using namespace std::chrono;
using namespace immagine;
using namespace std;

#define M_PI       3.14159265358979323846  

void filter(Kernel& kernel)
{
	int offset = kernel.width / 2;
	double stdv = 20.0;
	double r, s = 2.0 * stdv * stdv;  // Assigning standard deviation to 1.0
	double sum = 0.0;   // Initialization of sun for normalization
	for (int x = -1 * offset; x <= offset; x++) // Loop to generate 5x5 kernel
	{
		for (int y = -1 * offset; y <= offset; y++)
		{
			r = sqrt(x*x + y * y);
			kernel(x + offset, y + offset) = (exp(-(r*r) / s)) / (M_PI * s);
			sum += kernel(x + offset, y + offset);
		}
	}

	for (int i = 0; i < kernel.height; ++i) // Loop to normalize the kernel
		for (int j = 0; j < kernel.width; ++j)
			kernel(i, j) /= sum;

}
int
main(int argc, char** argv)
{
	Image image = image_load("D:/Ismail/images/10.png");

	auto start = high_resolution_clock::now();

	Kernel kernel = kernel_new(5, 5);
	filter(kernel);
	printf("-------------------\n");

	//type code here.
	Image img1 = kernel_conv(kernel, image);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());

	image_save("D:/Ismail/images/convolved_image.bmp", img1, FORMAT::BMP);

	image_free(image);
	image_free(img1);

	return 0;
}

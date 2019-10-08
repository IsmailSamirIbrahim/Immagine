#include <iostream>

#include "Immagine/Image.h"
#include "Immagine/Point_Processing.h"
#include "Immagine/Convolution.h"

#include <chrono>
#include <string>
#include <vld.h>

using namespace std::chrono;
using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	string file_path = string(IMAGE_DIR) + string("/images/log.bmp");
	Image image = image_load(file_path.c_str());
	
	auto start = high_resolution_clock::now();

	//type code here.
	
	Image img = image_logarithm_inv(image);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("Time = %lld  milliseconds\n", duration.count());

	string out_path = string(IMAGE_DIR) + string("/images/log_inv.bmp");
	image_save(out_path.c_str(), img, IMAGE_FORMAT::BMP);

	image_free(image);
	image_free(img);

	return 0;
}

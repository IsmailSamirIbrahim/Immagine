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

	string file_path = string(IMAGE_DIR) + string("/images/fruits.bmp");
	Image image = image_load(file_path.c_str());
	
	auto start = high_resolution_clock::now();

	//type code here.
	
	Image img = image_binarize(image);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  seconds\n", duration.count());

	string out_path = string(IMAGE_DIR) + string("/images/binraized.bmp");
	image_save(out_path.c_str(), img, IMAGE_FORMAT::BMP);

	image_free(image);
	image_free(img);

	return 0;
}

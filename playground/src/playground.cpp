#include <iostream>

#include "Immagine/Image.h"
#include "Immagine/Point_Processing.h"
#include "Immagine/Convolution.h"
#include "Immagine/Disjoint_Set.h"

#include <chrono>
#include <string>

//#include <vld.h>

using namespace std::chrono;
using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{

	string file_path = string(IMAGE_DIR) + string("/images/1.png");
	Image image = image_load(file_path.c_str());
	
	auto start = high_resolution_clock::now();

	//type code here.

	Image img1 = image_grayscale(image);
	Image img2 = image_binarize(img1);

	Image result = image_connected_component(img2);


	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  seconds\n", duration.count());

	string out_path = string(IMAGE_DIR) + string("/images/result.bmp");
	image_save(out_path.c_str(), result, IMAGE_FORMAT::BMP);

	return 0;
}

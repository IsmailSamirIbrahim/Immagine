#include <iostream>

#include "Immagine/Image.h"
#include "Immagine/Quadtree.h"

#include <chrono>

using namespace std::chrono;
using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	Image img = image_load("D:/test.bmp");
	
	auto start = high_resolution_clock::now();

	//type code here.
	Quadtree q = quadtree_build(img, Region{ 0, 0, img.width, img.height });

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());

	return 0;
}

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
	auto start = high_resolution_clock::now();

	//type code here.

	Image image = image_new(4, 4, 1);

	//set the image data
	//NW
	image(0, 0, 0) = 1; image(0, 1, 0) = 1;
	image(1, 0, 0) = 1; image(1, 1, 0) = 1;

	//NE
	image(0, 2, 0) = 0; image(0, 3, 0) = 0;
	image(1, 2, 0) = 1; image(1, 3, 0) = 0;

	//SW
	image(2, 0, 0) = 0; image(2, 1, 0) = 0;
	image(3, 0, 0) = 0; image(3, 1, 0) = 0;

	//SE
	image(2, 2, 0) = 1; image(2, 3, 0) = 0;
	image(3, 2, 0) = 0; image(3, 3, 0) = 0;

	Quadtree quadtree = quadtree_new(image);

	quadtree_build(quadtree, image);

	quadtree_free(quadtree);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());

	return 0;
}

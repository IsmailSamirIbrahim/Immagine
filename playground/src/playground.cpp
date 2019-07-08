#include <iostream>

#include "Immagine/Image.h"
#include "Immagine/Quadtree.h"
#include "Immagine/Utils.h"

#include <chrono>

using namespace std::chrono;
using namespace immagine;
using namespace std;

int
main(int argc, char** argv)
{
	Image img = image_load("D:/very_big.jpg");

	auto start = high_resolution_clock::now();

	//type code here.

	//Image image = image_new(4, 4, 1);

	////set the image data
	////NW
	//image(0, 0, 0) = 1; image(0, 1, 0) = 1;
	//image(1, 0, 0) = 1; image(1, 1, 0) = 1;

	////NE
	//image(0, 2, 0) = 0; image(0, 3, 0) = 0;
	//image(1, 2, 0) = 1; image(1, 3, 0) = 0;

	////SW
	//image(2, 0, 0) = 0; image(2, 1, 0) = 0;
	//image(3, 0, 0) = 0; image(3, 1, 0) = 0;

	////SE
	//image(2, 2, 0) = 1; image(2, 3, 0) = 0;
	//image(3, 2, 0) = 0; image(3, 3, 0) = 0;

	//Quadtree quadtree = quadtree_build(image);

	//quadtree_free(quadtree);


	Quadtree quadtree = quadtree_build(img);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  second\n", duration.count());

	std::vector<Region> regions = quadtree_simulate(quadtree, img);
	for (Region region : regions)
	{
		if (region.width > 1 && region.height > 1)
			draw_regione(img, region);
	}

	quadtree_free(quadtree);

	image_save("D:/aaa.bmp", img, BMP);

	return 0;
}

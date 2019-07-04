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

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	printf("Time = %lld  millisecond\n", duration.count());

	return 0;
}

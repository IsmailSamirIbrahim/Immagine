#pragma once

#include "Immagine/Exports.h"
#include "Immagine/Image.h"

#include <vector>

#define THRESHOLD 0.6
namespace immagine
{
	//Quadtree
	typedef struct IQuadtree* Quadtree;

	IMMAGINE_EXPORT Quadtree
	quadtree_build(const Image& image);

	IMMAGINE_EXPORT std::vector<Region>
	quadtree_simulate(Quadtree Quadtree, Image& image);

	IMMAGINE_EXPORT void
	quadtree_free(Quadtree quadtree);
}
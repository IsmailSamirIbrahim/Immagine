#pragma once

#include "Immagine/Exports.h"
#include "Immagine/Image.h"

#define THRESHOLD 0.6
namespace immagine
{
	//Quadtree
	typedef struct IQuadtree* Quadtree;

	IMMAGINE_EXPORT Quadtree
	quadtree_build(const Image& image);

	IMMAGINE_EXPORT void
	quadtree_free(Quadtree quadtree);
}
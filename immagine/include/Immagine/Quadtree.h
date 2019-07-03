#pragma once

#include "Immagine/Exports.h"
#include "Immagine/Image.h"
#include "Immagine/Imagedef.h"

#define THRESHOLD 0.6
namespace immagine
{
	//Node
	typedef struct INode* Node;

	struct INode
	{
		Color color;
		Region region;
		Node octants[4];
	};

	Node
	quadtree_node_build(const Image& image, Region region);

	void
	quadtree_node_free(Node node);

	//Quadtree
	typedef struct IQuadtree* Quadtree;

	struct IQuadtree
	{
		Node root;
	};

	IMMAGINE_EXPORT Quadtree
	quadtree_build(const Image& image, Region region);

	IMMAGINE_EXPORT void
	quadtree_free(Quadtree node);
}
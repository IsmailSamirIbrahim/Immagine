#include "Immagine/Quadtree.h"
#include "Immagine\Utils.h"

#include <corecrt_malloc.h>

namespace immagine
{
	//Node
	Node
	quadtree_node_build(const Image& image, Region region)
	{
		Node self = (Node)::malloc(sizeof(INode));

		self->region = region;

		if ((region.width == 1) || (region.height == 1) || (region_measure_detail(image, region) <= THRESHOLD))
			self->color = region_average_color(image, region);
		else
		{
			self->octants[0] = quadtree_node_build(image, Region{ region.x, region.y, region.width / 2, region.height / 2 });
			self->octants[1] = quadtree_node_build(image, Region{ region.x + region.width / 2, region.y, region.width - region.width / 2, region.height / 2 });
			self->octants[2] = quadtree_node_build(image, Region{ region.x, region.y + region.height / 2, region.width / 2, region.height - region.height / 2 });
			self->octants[3] = quadtree_node_build(image, Region{ region.x + region.width / 2, region.y + region.height / 2, region.width - region.width / 2, region.height - region.height / 2 });
		}

		return self;
	}

	void
	quadtree_node_free(Node node)
	{
		if (node == nullptr)
			return;

		for (size_t i = 0; i < 4; ++i)
			quadtree_node_free(node->octants[i]);

		::free(node);
	}
	
	
	//Quadtree
	Quadtree
	quadtree_build(const Image& image, Region region)
	{
		Quadtree self = (Quadtree)::malloc(sizeof(IQuadtree));

		self->root = quadtree_node_build(image, region);

		return self;
	}

	void
	quadtree_free(Quadtree node)
	{
		quadtree_node_free(node->root);
		::free(node);
	}
}
#include "Immagine/Quadtree.h"
#include "Immagine/Utils.h"

#include <stdlib.h>

namespace immagine
{
	//Node
	typedef struct INode* Node;

	struct INode
	{
		enum Kind { KIND_NONE, KIND_NON_LEAF, KIND_LEAF };

		Kind kind;
		Color color;
		Region region;
		Node childrens[4];
	};

	inline static Node
	node_new(INode::Kind kind, Region region)
	{
		Node self = (Node)::malloc(sizeof(INode));

		self->kind = kind;
		self->color = Color{ 0, 0, 0 };
		self->region = region;
		
		for (Node child: self->childrens)
			child = nullptr;

		return self;
	}

	inline static void
	node_free(Node node)
	{
		switch (node->kind)
		{
		case INode::KIND_LEAF:
			::free(node);
			break;

		case INode::KIND_NON_LEAF:
			for (Node child: node->childrens)
				node_free(child);
			::free(node);
			break;

		default:
			assert(false && "Unreachable state");
			break;
		}
	}

	inline static void
	node_build(Node node, const Image& image)
	{
		Region region = node->region;
		float measure_detail = region_measure_detail(image, region);

		//quadtree stop branching when these conditions are true.
		if (/*region.width == 1 || region.height == 1 || */measure_detail < THRESHOLD)
		{
			node->kind = INode::KIND_LEAF;
			node->color = regione_average_color(image, region);
		}
		else
		{
			//create the childrens nodes for this Non-Leaf node
			node->childrens[0] = node_new(INode::KIND_NON_LEAF, Region{ region.x, region.y, region.width / 2, region.height / 2 });
			node->childrens[1] = node_new(INode::KIND_NON_LEAF, Region{ region.x + region.width / 2, region.y, region.width - region.width / 2, region.height / 2 });
			node->childrens[2] = node_new(INode::KIND_NON_LEAF, Region{ region.x, region.y + region.height / 2, region.width / 2, region.height - region.height / 2 });
			node->childrens[3] = node_new(INode::KIND_NON_LEAF, Region{ region.x + region.width / 2, region.y + region.height / 2, region.width - region.width / 2, region.height - region.height / 2 });

			for (Node child : node->childrens)
				node_build(child, image);
		}
	}

	//Quadtree
	struct IQuadtree
	{
		Node root;
	};
	
	Quadtree
	quadtree_build(const Image& image)
	{
		Quadtree self = (Quadtree)::malloc(sizeof(IQuadtree));

		self->root = node_new(INode::KIND_NON_LEAF, Region{ 0, 0, image.width, image.height });

		node_build(self->root, image);

		return self;
	}

	inline static void
	simulate(Node node, Image& image, std::vector<Region>& regions)
	{
		switch (node->kind)
		{
		case INode::KIND_LEAF:
			regions.push_back(node->region);
			break;

		case INode::KIND_NON_LEAF:
			for (Node child : node->childrens)
				simulate(child, image, regions);
			break;

		default:
			break;
		}
	}

	std::vector<Region>
	quadtree_simulate(Quadtree Quadtree, Image& image)
	{
		std::vector<Region> regions;
		simulate(Quadtree->root, image, regions);
		return regions;
	}

	void
	quadtree_free(Quadtree quadtree)
	{
		node_free(quadtree->root);
	}
}
#include "Immagine/Quadtree.h"
#include "Immagine\Utils.h"

#include <stdlib.h>

namespace immagine
{
	//Node
	typedef struct INode* Node;

	struct INode
	{
		enum Kind { KIND_NONE, KIND_NON_LEAF, KIND_LEAF };

		Kind kind;
		Region region;
		int8_t value;
		Node octants[4];
	};

	inline static Node
	node_new(INode::Kind kind, Region region)
	{
		Node self = (Node)::malloc(sizeof(INode));

		self->kind = kind;
		self->region = region;
		self->value = -1;

		for (Node octant: self->octants)
			octant = nullptr;

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
			for (Node octant: node->octants)
				node_free(octant);
			::free(node);
			break;

		default:
			assert(false && "Unreachable state");
			break;
		}
	}

	inline static void
	node_root_build(Node node, const Image& image)
	{
		Region region = node->region;
		int8_t value = node_is_leaf(image, region);

		//quadtree stop branching when these conditions is true.
		if (region.width == 1 || region.height == 1 || value != -1)
		{
			node->kind = INode::KIND_LEAF;
			node->value = value;
		}
		else
		{
			//create the octants nodes for this Non-Leaf node
			node->octants[0] = node_new(INode::KIND_NON_LEAF, Region{ region.x, region.y, region.width / 2, region.height / 2 });
			node->octants[1] = node_new(INode::KIND_NON_LEAF, Region{ region.x + region.width / 2, region.y, region.width - region.width / 2, region.height / 2 });
			node->octants[2] = node_new(INode::KIND_NON_LEAF, Region{ region.x, region.y + region.height / 2, region.width / 2, region.height - region.height / 2 });
			node->octants[3] = node_new(INode::KIND_NON_LEAF, Region{ region.x + region.width / 2, region.y + region.height / 2, region.width - region.width / 2, region.height - region.height / 2 });

			for (Node octant: node->octants)
				node_root_build(octant, image);
		}
	}

	//Quadtree
	struct IQuadtree
	{
		Node root;
	};
	
	Quadtree
	quadtree_new(const Image& image)
	{
		Quadtree self = (Quadtree)::malloc(sizeof(IQuadtree));

		self->root = node_new(INode::KIND_NON_LEAF, Region{ 0, 0, image.width, image.height });

		return self;
	}

	void
	quadtree_free(Quadtree quadtree)
	{
		node_free(quadtree->root);
	}

	void
	quadtree_build(Quadtree quadtree, const Image& image)
	{
		node_root_build(quadtree->root, image);
	}
}
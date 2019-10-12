#pragma once
#include "Immagine/Disjoint_Set.h"

using namespace std;

namespace immagine
{

	std::unordered_map<uint8_t, Node> Disjoint_Set::map;

	Disjoint_Set
	disjoint_set_new()
	{
		Disjoint_Set self{};
		return self;
	}
	
	void
	disjoint_set_make(Disjoint_Set self, uint8_t data)
	{
		Node node = (Node)::malloc(sizeof(Node));
		node->data = data;
		node->rank = 0;
		node->parent = node;

		self.map[data] = node;
	}

	inline static Node
	_disjoint_set_find(Node node)
	{
		Node parent = node->parent;
		if (parent == node)
			return parent;

		//path compression
		node->parent = _disjoint_set_find(node->parent);

		return node->parent;
	}

	uint8_t
	disjoint_set_find(Disjoint_Set self, uint8_t data)
	{
		Node node = self.map.find(data)->second;
		return _disjoint_set_find(node)->data;
	}

	void
	disjoint_set_union(Disjoint_Set self, uint8_t data1, uint8_t data2)
	{
		Node node1 = self.map.find(data1)->second;
		Node node2 = self.map.find(data2)->second;

		Node parent1 = _disjoint_set_find(node1);
		Node parent2 = _disjoint_set_find(node2);

		//they are part of the same set
		if (parent1->data == parent2->data)
			return;

		if (parent1->rank >= parent2->rank)
		{
			parent1->rank = (parent1->rank == parent2->rank) ? parent1->rank + 1 : parent1->rank;
			parent2->parent = parent1;
		}
		else
			parent1->parent = parent2;
	}
}
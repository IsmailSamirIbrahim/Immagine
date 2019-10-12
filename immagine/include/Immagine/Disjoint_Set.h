#pragma once

#include "Immagine/Exports.h"

#include <stdint.h>
#include <unordered_map>

namespace immagine
{
	typedef struct INode* Node;
	struct INode
	{
		uint8_t rank;
		uint8_t data;
		Node parent;
	};

	struct Disjoint_Set
	{
		static std::unordered_map<uint8_t, Node> map;
	};

	IMMAGINE_EXPORT Disjoint_Set
	disjoint_set_new();

	IMMAGINE_EXPORT void
	disjoint_set_make(Disjoint_Set self, uint8_t data);

	IMMAGINE_EXPORT uint8_t
	disjoint_set_find(Disjoint_Set self, uint8_t data);

	IMMAGINE_EXPORT void
	disjoint_set_union(Disjoint_Set self, uint8_t data1, uint8_t data2);
}
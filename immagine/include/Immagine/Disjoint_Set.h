#pragma once

#include "Immagine/Exports.h"

#include <stdint.h>
#include <unordered_map>

namespace immagine
{
	struct Disjoint_Set
	{
		uint16_t* arr;
		uint16_t* size;
	};

	IMMAGINE_EXPORT Disjoint_Set
	disjoint_set_new(size_t size);

	IMMAGINE_EXPORT void
	disjoint_set_free(Disjoint_Set& self);

	IMMAGINE_EXPORT uint16_t
	disjoint_set_find_root(Disjoint_Set self, uint16_t data);

	IMMAGINE_EXPORT void
	disjoint_set_union(Disjoint_Set self, uint16_t data1, uint16_t data2);
}
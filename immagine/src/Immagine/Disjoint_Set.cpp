#pragma once
#include "Immagine/Disjoint_Set.h"

using namespace std;

namespace immagine
{
	Disjoint_Set
	disjoint_set_new(size_t size)
	{
		Disjoint_Set self{};

		self.arr = (uint16_t*)::malloc(size * sizeof(uint16_t));
		self.size = (uint16_t*)::malloc(size * sizeof(uint16_t));

		for (size_t i = 0; i < size; ++i)
		{
			self.arr[i] = i;
			self.size[i] = 1;
		}

		return self;
	}

	void
	disjoint_set_free(Disjoint_Set& self)
	{
		::free(self.arr);
		::free(self.size);
	}
	
	uint16_t
	disjoint_set_find_root(Disjoint_Set self, uint16_t data)
	{
		while (self.arr[data] != data)
		{
			data = self.arr[data];
		}
		return data;
	}

	void
	disjoint_set_union(Disjoint_Set self, uint16_t data1, uint16_t data2)
	{
		uint16_t root_data1 = disjoint_set_find_root(self, data1);
		uint16_t root_data2 = disjoint_set_find_root(self, data2);

		if (self.size[root_data1] < self.size[root_data2])
		{
			self.arr[root_data1] = self.arr[root_data2];
			self.size[root_data2] += self.size[root_data1];
		}
		else
		{
			self.arr[root_data2] = self.arr[root_data1];
			self.size[root_data1] += self.size[root_data2];
		}
	}
}
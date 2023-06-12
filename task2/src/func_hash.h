#pragma once
#include <string>
#include <list>
#include "mbf16.h"
#include "stdint.h"

using namespace std;

namespace mbf16
{	
	struct func_data
	{
		mbf7 func;
		uint8_t* func_bit_values;
		uint64_t func_level_fill;
		uint64_t func_level_special_fill;
		uint8_t func_num_bits;		

		uint64_t binode_hash;

		uint8_t* special_node_values;
		uint8_t special_node_count;

		uint64_t* special_node_vertices_hash;
		uint64_t* special_node_neighbours_hash;

		uint64_t* vertice_hash;
		bool SpecialBit(const int bit);
		func_data();
		~func_data();
	};


	class func_hash
	{
	public:
		func_hash(func_data& data);		
		uint64_t hash(func_data& data) const;

		void vertice_hash(func_data & data, int const vertice);
		void node_vertice_hash(func_data& data, int const node_id);
		void binode_hash(func_data& data, uint8_t vertice1, uint8_t vertice2);
		void node_neighbour_hash(func_data& data, uint8_t node1, uint8_t node2);
		//void dump() const;
	};
}
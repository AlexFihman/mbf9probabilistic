#include <iostream>
#include "mbf16.h"
#include "stdint.h"
#include "func_hash.h"
#include "hiso_hash.h"
#include "time_ms.h"
#include <stdlib.h>

namespace mbf16
{

	void func_hash::node_neighbour_hash(func_data& data, uint8_t node1, uint8_t node2)
	{
		uint8_t node1node = data.special_node_values[node1];
		uint8_t node2node = data.special_node_values[node2];

		uint8_t distance = level_of_item[node1node ^ node2node];

		uint64_t vertices_hash = data.special_node_vertices_hash[node1];
		uint64_t another_hash = data.special_node_vertices_hash[node2];

		
		uint64_t first_node_vertices = 0;
		uint64_t second_node_vertices = 0;
		uint64_t both_nodes_vertices = 0;

		for (int i = 0; i < 7; i++)
		{
			int ourbit = 1 << i;
			if ((node1node & ourbit) && !(node2node & ourbit))
			{
				first_node_vertices += data.vertice_hash[i];
			}

			if (!(node1node & ourbit) && (node2node & ourbit))
			{
				second_node_vertices += data.vertice_hash[i];
			}

			if ((node1node & ourbit) && (node2node & ourbit))
			{
				both_nodes_vertices += data.vertice_hash[i];
			}
		}

				
		HISO_HASH_CTX ctx;
		hiso_hash_init(ctx);
		hiso_hash_upd(ctx, distance);
		hiso_hash_upd(ctx, vertices_hash);
		hiso_hash_upd(ctx, another_hash);

		hiso_hash_upd(ctx, first_node_vertices);
		hiso_hash_upd(ctx, second_node_vertices);
		hiso_hash_upd(ctx, both_nodes_vertices);

		uint64_t result = hiso_hash_result(ctx);

		data.special_node_neighbours_hash[node1] += result;


		hiso_hash_init(ctx);
		hiso_hash_upd(ctx, distance);

		hiso_hash_upd(ctx, another_hash);
		hiso_hash_upd(ctx, vertices_hash);

		hiso_hash_upd(ctx, second_node_vertices);
		hiso_hash_upd(ctx, first_node_vertices);		
		hiso_hash_upd(ctx, both_nodes_vertices);

		result = hiso_hash_result(ctx);
		data.special_node_neighbours_hash[node2] += result;
	}
	

	func_hash::func_hash(func_data& data)
	{
		for (int i = 0; i < 128; i++)
			data.func_bit_values[i] = data.func.GetBit(i);

		data.func_num_bits = 0;
		data.special_node_count = 0;

		data.func_level_fill = 0;
		data.func_level_special_fill = 0;
		data.special_node_count = 0;

		for (int i=0;i<128;i++)
		{
			if (data.func_bit_values[i])
			{
				int level = level_of_item[i];
				data.func_num_bits++;
				data.func_level_fill += 1ULL << (level*8);

				if (data.SpecialBit(i))
				{
					data.special_node_values[data.special_node_count] = (uint8_t)i;
					data.special_node_count++;
					data.func_level_special_fill += 1ULL << (level*8);
				}				
			}
		}

		//vertice hash
		for (int i = 0; i < 7; i++)
		{
			vertice_hash(data, i);
		}

		for (int i = 0; i < data.special_node_count; i++)
		{
			node_vertice_hash(data, i);
		}

		for (int i=0; i < data.special_node_count; i++)
		{
			data.special_node_neighbours_hash[i] = 0;
		}

		for (int i = 0; i < data.special_node_count - 1; i++)
			for (int j = i + 1; j < data.special_node_count; j++)
			{
				if (i != j)
					node_neighbour_hash(data,i,j);
			}

		data.binode_hash = 0;
		for (int i=0; i<6; i++)
			for (int j=i+1; j<7; j++)
			{
				binode_hash(data,i,j);
			}
	}

	void func_hash::vertice_hash(func_data& data, int const vertice)
	{
		uint64_t vertice_bits = 0;
		uint64_t vertice_special_bits = 0;

		uint64_t vertice_level_fill = 0;
		uint64_t vertice_level_special_fill = 0;

		uint8_t ourbit = 1 << vertice;

		for (int i = 0; i < 128; i++)
		{
			if (i & ourbit)
				if (data.func_bit_values[i])
				{
					vertice_bits++;
					vertice_level_fill += 1ULL << (level_of_item[i] * 8);
				}
		}

		for (int i = 0; i< data.special_node_count; i++)
		{
			uint8_t node = data.special_node_values[i];
			if (node & ourbit)
			{
				vertice_special_bits++;
				int level = level_of_item[node];
				vertice_level_special_fill += 1ULL << (level * 8);
			}
		}

		HISO_HASH_CTX ctx;
		hiso_hash_init(ctx);
		hiso_hash_upd(ctx, vertice_bits);
		hiso_hash_upd(ctx, vertice_level_fill);
		hiso_hash_upd(ctx, vertice_special_bits);
		hiso_hash_upd(ctx, vertice_level_special_fill);
		data.vertice_hash[vertice] = hiso_hash_result(ctx);
#ifdef _DEBUG
		cout << "vertice: " << vertice << " hash: " << data.vertice_hash[vertice] << endl;
#endif
	}

	void func_hash::node_vertice_hash(func_data& data, int const node_id)
	{
		uint8_t node = data.special_node_values[node_id];
			
		int level = level_of_item[node];

		data.special_node_vertices_hash[node_id] = 0;
		
		for (int j = 0; j < 7; j++)
		{
			int ourbit = 1 << j;
			if (node & ourbit)
			{
				data.special_node_vertices_hash[node_id] += data.vertice_hash[j];
			}
		}	
#ifdef _DEBUG
		cout << "special_node_vertices_hash node: " << (int)node_id << " hash: " << data.special_node_vertices_hash[node_id] << endl;
#endif
	}

	uint64_t func_hash::hash(func_data& data) const
	{
		HISO_HASH_CTX ctx;
		hiso_hash_init(ctx);
		hiso_hash_upd(ctx, data.func_num_bits);
		hiso_hash_upd(ctx, data.special_node_count);

		hiso_hash_upd(ctx, data.func_level_fill);
		hiso_hash_upd(ctx, data.func_level_special_fill);
		hiso_hash_upd(ctx, data.binode_hash);

		uint64_t result = hiso_hash_result(ctx);


		for (int i=0;i<7;i++)
			result += data.vertice_hash[i];

		for (int i = 0; i < data.special_node_count; i++)
		{
			result += data.special_node_vertices_hash[i];
			result += data.special_node_neighbours_hash[i];
		}
			
		return result;
	}
	

	
	void func_hash::binode_hash(func_data& data, uint8_t vertice1, uint8_t vertice2)
	{		
		uint64_t vhash1 = data.vertice_hash[vertice1];
		uint64_t vhash2 = data.vertice_hash[vertice2];
		uint64_t vhash = vhash1 + vhash2;
		uint64_t vhash3 = vhash1 * vhash2;

		uint64_t node_neighbour_hash1 = 0;
		uint64_t node_vertices_hash1 = 0;
		uint64_t node_neighbour_hash2 = 0;
		uint64_t node_vertices_hash2 = 0;
		uint64_t node_neighbour_hash3 = 0;
		uint64_t node_vertices_hash3 = 0;
		
		int ourbit1 = (1 << vertice1);
		int ourbit2 = (1 << vertice2);

		for (int i = 0; i < data.special_node_count; i++)
		{
			uint8_t const node = data.special_node_values[i];
			bool b1 = false;

			if (ourbit1 == (node & ourbit1))
			{
				node_neighbour_hash1 += data.special_node_neighbours_hash[i];
				node_vertices_hash1 += data.special_node_vertices_hash[i];
				b1 = true;
			}

			bool b2 = false;
			if (ourbit2 == (node & ourbit2))
			{
				node_neighbour_hash2 += data.special_node_neighbours_hash[i];
				node_vertices_hash2 += data.special_node_vertices_hash[i];
				b2 = true;
			}

			if (b1 && b2)
			{
				node_neighbour_hash3 += data.special_node_neighbours_hash[i];
				node_vertices_hash3 += data.special_node_vertices_hash[i];
			}
		}
		uint64_t node_neighbour_hash = node_neighbour_hash1 * vhash1 + node_neighbour_hash2 * vhash2 + node_neighbour_hash3 * vhash3;
		uint64_t node_vertices_hash = node_vertices_hash1 * vhash1 + node_vertices_hash2 * vhash2 + node_vertices_hash3 * vhash3;

		HISO_HASH_CTX ctx;
		hiso_hash_init(ctx);
		hiso_hash_upd(ctx, vhash);
		hiso_hash_upd(ctx, node_neighbour_hash);
		hiso_hash_upd(ctx, node_vertices_hash);
#ifdef _DEBUG
        cout << "vertice1 " << (int)vertice1 << " vertice2 " << (int)vertice2 << " vhash " << vhash << " node_vertices_hash " << node_vertices_hash << endl;
#endif
		data.binode_hash += hiso_hash_result(ctx);
	}	

	bool func_data::SpecialBit(const int bit)
	{
		bool result = true;
		for (int loop = 0; loop < 8; loop++)
		{
			int test = bit ^ (1 << loop);
			if (test < bit)
				if (func_bit_values[test])
				{
					result = false;
					break;
				}
		}
		return result;
	}

	func_data::func_data()
	{
		special_node_values = (uint8_t*)malloc(sizeof(uint8_t) * 128);
		vertice_hash = (uint64_t*)malloc(sizeof(uint64_t) * 7);
		func_bit_values = (uint8_t*)malloc(sizeof(uint8_t) * 128);
		special_node_vertices_hash = (uint64_t*)malloc(sizeof(uint64_t) * 128);
		special_node_neighbours_hash = (uint64_t*)malloc(sizeof(uint64_t) * 128);
	}

	func_data::~func_data()
	{
		delete special_node_values;
		delete vertice_hash;
		delete func_bit_values;
		delete special_node_vertices_hash;
		delete special_node_neighbours_hash;
	}
}
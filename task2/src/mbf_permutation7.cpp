#include <iostream>
#include <algorithm>
#include "mbf16.h"
#include <cstring>
#include "mbf_permutation7.h"

namespace mbf16
{	

	uint8_t** data_permutation;

	char** data;

	//char** data_small;

	void mbf_permutation7::prepare_symm(mbf7 const val)
	{
		for (int j = 0; j < 128; j++)
			bits[j] = val.GetBitOld(j);
	}

	mbf7 mbf_permutation7::symm(mbf7 const val, int symm)
	{
		mbf7 res = { 0, 0 };

		for (int j = 0; j < 128; j++)
		{
			if (bits[j])
				res.SetBit(data_permutation[symm][j]);
		}
		return res;
	}

	mbf7 mbf_permutation7::min_symmetry(mbf7 const val)
	{		
		//std::cout << "min_symmetry" << std::endl;
		prepare_symm(val);
		mbf7 minval = val;
		for (int i = 1; i < 5040; i++)
		{
			mbf7 res = symm(val, i);
			if (res.Compare(minval) < 0)
			{
				minval = res;
				//std::cout << "i=" << i << std::endl;
			}
		}
		return minval;
	}	

	void mbf_permutation7::init_symm7()
	{
		data = new char*[5040];
		for (int i = 0; i < 5040; i++)
			data[i] = new char[7];

		for (int i = 0; i < 7; i++)
		{
			data[0][i] = i;
		}
		for (int i = 1; i < 5040; i++)
		{
			memcpy(data[i], data[i - 1], sizeof(char) * 7);
			std::next_permutation(data[i], data[i] + 7);
		}

		data_permutation = new uint8_t*[5040];
		for (int i = 0; i < 5040; i++)
		{
			data_permutation[i] = new uint8_t[128];
			for (int j = 0; j < 128; j++)
			{
				uint8_t res = 0;
				for (int t = 0; t < 7; t++)
				{
					res |= ((j >> t) & 1) << data[i][t];
				}
				data_permutation[i][j] = res;
			}
		}
	}

}
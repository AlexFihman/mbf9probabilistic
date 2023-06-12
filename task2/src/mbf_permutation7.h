#pragma once

#include "stdint.h"
#include "mbf16.h"

namespace mbf16
{	


	class mbf_permutation7
	{		
	private:
		char bits[128];
	public:
		void prepare_symm(mbf7 const val);

		mbf7 symm(mbf7 const val, int symm);

		mbf7 min_symmetry(mbf7 const val);

		void init_symm7();
	};

}
#pragma once
#include "mbf16.h"
#include <cstdint>

namespace mbf16
{
	struct squares_state {
		uint64_t ctr;
		uint64_t key;
	};

	extern void rand_init(squares_state& state, int key);

	extern int fn6(squares_state& state);

	extern mbf7 makef7(squares_state& state);
	
	extern mbf8 makef8(squares_state& state);
}

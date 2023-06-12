#pragma once
#include "mbf16.h"
#include "SFMT.h"

namespace mbf16
{
	extern void rand_init(uint64_t seed, sfmt_t& mt);

	extern int fn6(sfmt_t& mt);

	extern mbf7 makef7(sfmt_t& mt);
}
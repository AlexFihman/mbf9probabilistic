#pragma once
#include "stdint.h"

struct HISO_HASH_CTX
{
	uint64_t x1;
	uint64_t x2;
	uint64_t x3;
} ;

extern void hiso_hash_init(HISO_HASH_CTX& ctx);

extern void hiso_hash_upd(HISO_HASH_CTX & ctx, uint64_t value);

extern uint64_t hiso_hash_result(HISO_HASH_CTX & ctx);
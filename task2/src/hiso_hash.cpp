#include "hiso_hash.h"

void hiso_hash_init(HISO_HASH_CTX & ctx)
{
	ctx.x1 = 6148914691236517205ULL;
	ctx.x2 = 3689348814741910323ULL;
	ctx.x3 =  970881267037344821ULL;
}

uint64_t rotl(uint64_t x, int pos)
{
	return (x << pos) | (x >> (64 - pos));
}

void hiso_hash_upd(HISO_HASH_CTX & ctx, uint64_t value)
{
	ctx.x3 ^= value + rotl(ctx.x1,22) + rotl(ctx.x2, 53);
	ctx.x2 ^= rotl(ctx.x3, 13) + rotl(ctx.x1, 7);
	ctx.x1 ^= rotl(ctx.x2, 17) + rotl(ctx.x3, 21);
}

uint64_t hiso_hash_result(HISO_HASH_CTX & ctx)
{
	return ctx.x1 ^ ctx.x2 ^ ctx.x3;
}

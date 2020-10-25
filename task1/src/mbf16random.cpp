#define SFMT_MEXP 19937

#include "mbf16.h"
#include "mbf16random.h"
#include <stdlib.h>
#include "squares.h"
#include "keys.h"

namespace mbf16
{
	

	int fn6(squares_state& state){
		int r;
		do { r = squares(state.ctr, state.key) & 8388607; state.key++;} while (r >= d6);
		return r;
	}

	mbf7 makef7(squares_state& state)
	{
		mbf7 t;
		mbf x1, x2;
		do{
			x1 = f6[fn6(state)];
			x2 = f6[fn6(state)];
		} while (!cmp6(x1, x2));
		t.f1 = x1;
		t.f2 = x2;
		return t;
	}
	
	mbf8 makef8(squares_state& state)
	{
		mbf8 t;
		mbf7 x1, x2;
		do{
			x1 = makef7(state);
			x2 = makef7(state);
		} while (!cmp7(x1, x2));
		t.f1 = x1;
		t.f2 = x2;
		return t;
	}
	
	

	//Marsaglia's xorshift*
	 /* The state must be seeded with a nonzero value. */

	uint64_t xorshift64star(uint64_t& x) {
		x ^= x >> 12; // a
		x ^= x << 25; // b
		x ^= x >> 27; // c
		return x * UINT64_C(2685821657736338717);
	}


	uint32_t* randbuf(uint64_t seed)
	{
		uint64_t x;

		uint32_t* arr = (uint32_t*)malloc(624 * 4);

		if (seed == 0)
			x = 0xFFFFFFFFFFFFFFFF;
		else
		    x = seed;

		for (int i = 0; i < 624; i++)
			arr[i] = (uint32_t)xorshift64star(x);
		/*
		HMODULE lib = LoadLibraryA("Advapi32.dll");
		BOOLEAN(APIENTRY*pfn)(PVOID, ULONG);
		pfn = (BOOLEAN(APIENTRY*)(PVOID, ULONG))GetProcAddress(lib, "SystemFunction036");
		if (!pfn) {
			exit(1);
		}
		if (!pfn((PVOID)arr, 624 * 4)){
			exit(1);
		}
		FreeLibrary(lib);
		*/
		return arr;
	}

	void rand_init(squares_state& state, int key)
	{
		state.ctr = keys[key];
		state.key = 0;
	}
}

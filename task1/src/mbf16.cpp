#include "mbf16.h"
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <limits>

#define CHAR_BITS 8

namespace mbf16
{
	int d0, d1, d2, d3, d4, d5, d6;
	mbf* f0;
	mbf* f1;
	mbf* f2;
	mbf* f3;
	mbf* f4;
	mbf* f5;
	mbf* f6;
	uint8_t level_of_item[128];

	template <typename T>
	T reverse(T n, size_t b = sizeof(T) * CHAR_BITS)
	{
		assert(b <= std::numeric_limits<T>::digits);

		T rv = 0;

		for (size_t i = 0; i < b; ++i, n >>= 1)
			rv = (rv << 1) | (n & 0x01);

		return rv;
	}

	mbf7 mbf7::operator&(const mbf7 x) const
	{
		mbf7 result;
		result.f1 = f1 & x.f1;
		result.f2 = f2 & x.f2;
		return result;
	}

	mbf7 mbf7::operator| (const mbf7 x) const
	{
		mbf7 result;
		result.f1 = f1 | x.f1;
		result.f2 = f2 | x.f2;
		return result;
	}

	mbf7 mbf7::bitneg() const
	{
		mbf7 result;
		result.f1 = reverse(~f2);
		result.f2 = reverse(~f1);
		return result;
	}
	

	bool cmp6(const mbf f1, const mbf f2)
	{
		return (((~f1) | f2) == 0xFFFFFFFFFFFFFFFFLL);
	}


	bool cmp7(const mbf7 f1,const mbf7 f2)
	{
		return cmp6(f1.f1, f2.f1) && cmp6(f1.f2, f2.f2);
	}

	bool cmp8(const mbf8 f1,const mbf8 f2)
	{
		return cmp7(f1.f1, f2.f1) && cmp7(f1.f2, f2.f2);
	}

	int fcount(mbf const * const f, const int d) {
		int i, j;
		int cnt = 0;
		for (i = 0; i < d; i++)
			for (j = i; j < d; j++) {
				//cout << "i=" << i << " j=" << j << " f[i]=" << f[i] << " f[j]=" << f[j] << " ((~f[i]) | f[j]) =" << ((~f[i]) | f[j] ) << "\n";
				if (((~f[i]) | f[j]) == 0xFFFFFFFFFFFFFFFFLL) { cnt++; }
			}
		return cnt;
	}

	mbf* makespace(const int size) {
		return (mbf*)malloc(size*sizeof(mbf));
	}

	void pushlevel(mbf * const fnew, mbf const * const f, const int d, const int lvl) {
		int i, j, r;
		r = 1 << lvl;
		int cnt = 0;
		for (i = 0; i < d; i++)
			for (j = i; j < d; j++) {
				//cout << "i=" << i << " j=" << j << " f[i]=" << f[i] << " f[j]=" << f[j] << " ((~f[i]) | f[j]) =" << ((~f[i]) | f[j] ) << "\n";
				if (((~f[i]) | f[j]) == 0xFFFFFFFFFFFFFFFFLL) {
					fnew[cnt] = (f[i] << r) | f[j];
					cnt++;
				}
			}
	}

	void makelevel(mbf*& fnext, mbf const * const f, int& dnext, const int d, const int lvl) {
		dnext = fcount(f, d);
		//std::cout << "d" << lvl + 1 << "=" << dnext << std::endl;
		fnext = makespace(dnext);
		pushlevel(fnext, f, d, lvl);
	}

	
	uint8_t count_bits(int i)
	{
		int result = 0;
		while (i > 0)
		{
			result += (i & 1);
			i  >>= 1;
		}
		return result;
	}

	void init()
	{
		d0 = 2;
		f0 = makespace(d0);
		f0[0] = 0;
		f0[1] = 1;

		makelevel(f1, f0, d1, d0, 0);
		makelevel(f2, f1, d2, d1, 1);
		makelevel(f3, f2, d3, d2, 2);
		makelevel(f4, f3, d4, d3, 3);
		makelevel(f5, f4, d5, d4, 4);
		makelevel(f6, f5, d6, d5, 5);

		for (int i=0;i<128;i++)
		{
			level_of_item[i] = count_bits(i);
		}
	}

	void mbf7::SetBit(uint8_t bit)
	{
		if (bit < 64)
			f2 |= 1ULL << bit;
		else
			f1 |= 1ULL << (bit - 64);
	}

	int mbf7::Compare(mbf7 another)
	{
		if (f1 < another.f1)
			return -1;
		if (f1 > another.f1)
			return 1;
		if (f2 < another.f2)
			return -1;
		if (f2 > another.f2)
			return 1;
		return 0;
	}

	unsigned char mbf7::GetBit(const int bit) const
	{
		int bit2 = 127 - bit;
		if (bit2 < 64)
			return (f2 >> bit2) & 1;
		else
			return (f1 >> (bit2 - 64)) & 1;
	}

	unsigned char mbf7::GetBitOld(const int bit) const
	{
		if (bit < 64)
			return (f2 >> bit) & 1;
		else
			return (f1 >> (bit - 64)) & 1;
	}

	bool mbf7::SpecialBit(const int bit) const
	{
		bool result = true;
		for (int loop = 0; loop < 8; loop++)
		{
			int test = bit ^ (1 << loop);
			if (test < bit)
				if (GetBit(test))
				{
					result = false;
					break;
				}
		}
		return result;
	}

	int find_f6(mbf f)
	{
		int start = 0;
		if (f6[start] == f)
			return start;
		int end = d6-1;
		if (f6[end] == f)
			return end;

		int m = (start + end) >> 1;
		mbf d = f6[m];

		while (d != f)
		{
			if (d > f)
				end = m;
			else if (d < f)
				start = m;
			m = (start + end) >> 1;
			d = f6[m];
		}
		return m;
	}
}
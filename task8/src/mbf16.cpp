#include "mbf16.h"
#include "TFastList.h"

Mbf16::Mbf16()
{
	int i, j;

	for (i = 0;i < d1;i++) {
		bitcountMacro(i, j);
		LevelOfItem[i] = j;
	}

	for (i = 0;i < d3 + 1;i++) {
		LevelDepth[i] = 0;
	}
	for (i = 0;i < d1;i++)
	{
		int lo = LevelOfItem[i];
		int ld = LevelDepth[lo];
		levellist[lo][ld] = i;
		LevelDepth[lo]++;
	}

	fact[0] = 1.0;
	for (int i = 1;i < 130;i++)
		fact[i] = fact[i - 1] * i;
	//for (i = 0;i < d1;i++) {
		//MainArray[i] = false;
		//MainArrayU[i] = d3 - LevelOfItem[i];
		//MainArrayL[i] = 0;
		//LinkCnt[i] = 0;
	//}

	//BlackList = new TFastList;
	//WhiteList = new TFastList;	
	//MakeListOfGreys();

	//for (i = 0;i < d3 + 1;i++) {
	//	Level[i] = 0;
	//}

}


double Mbf16::comb(int n, int c) {
	return fact[n] / fact[n - c] / fact[c];
}

Mbf16::~Mbf16()
{
}

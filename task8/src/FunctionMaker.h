#pragma once
#include "TFastList.h"
#include "mbf16.h"
#include <Random123/threefry.h>

class FunctionMaker
{
private:
	Mbf16* mbf16;
	int level;
	TFastList* upperList;
public:
	FunctionMaker(Mbf16* m, int level, TFastList* list);
	double getS(threefry4x32_ctr_t& ctr, threefry4x32_key_t key);
};


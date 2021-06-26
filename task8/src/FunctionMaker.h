#pragma once
#include "TFastList.h"
#include "mbf16.h"

class FunctionMaker
{
private:
	Mbf16* mbf16;
	int level;
	TFastList* upperList;
public:
	FunctionMaker(Mbf16* m, int level, TFastList* list);
	double getS();
};


#include "FunctionMaker.h"
#include "TFastList.h"
#include "mbf16.h"


FunctionMaker::FunctionMaker(Mbf16* m, int level, TFastList* list) : mbf16(m), upperList(list), level(level)
{
	//this->mbf16 = m;
	//this->upperList = list;
	//this->level = level;
}

double FunctionMaker::getS(threefry4x32_ctr_t& ctr, threefry4x32_key_t key)
{
	if (mbf16->LevelDepth[level] == upperList->Count)
		return 1.0;

	double s = 0.0;
	int currLevel = level + 1;

	TFastList currList = TFastList();
	TFastList newList = TFastList();
	for (int j = 0;j < mbf16->LevelDepth[currLevel];j++)
		currList.Add(mbf16->levellist[currLevel][j]);

	for (int i = 0;i < upperList->Count;i++) {
		int item = upperList->Items[i];	

		for (int j = 0;j < d3;j++) {
			int itemx = item | (1 << j);
			if (itemx != item) {
				currList.Delete(itemx);
				newList.Add(itemx);
			}
		}		
	}

	int avail_to_fill = currList.Count;
	if (avail_to_fill == 0)
		return 1.0;

	if (currLevel == d3) {
		s += 1 << avail_to_fill;
	}
	else {
		s += 1.0;// i == avail_to_fill

		for (int i = 0;i < avail_to_fill;i++) {
			FunctionMaker nextFM = FunctionMaker(mbf16, currLevel, &newList);
			s += mbf16->comb(avail_to_fill, i) * nextFM.getS(ctr, key);
			
			ctr.v[0]++;
			threefry4x32_ctr_t rand = threefry4x32(ctr, key);

			int rand_item = rand.v[0] % currList.Count;
			int item = currList.Items[rand_item];
			newList.Add(item);
			currList.Delete(item);
		}
	}	

	return s;
}

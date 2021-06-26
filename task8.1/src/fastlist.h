#ifndef fastlist
#define fastlist
#include "mtrand.h"

class TFastList{
private:
	static MTRand mt;
public:
	TFastList();
	int           Count;
	int  Arr[65536];
	int  Items[65536];

	void  Add(int i);
	void  Delete(int i);
	void  Clear();
	int  RandItem();
	void  Assign(TFastList* lst);
};
#endif

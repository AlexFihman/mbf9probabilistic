#include <Random123/threefry.h>
#include "fastlist.h"
#include "string.h"
#include "assert.h"

TFastList::TFastList(){
   Count = 0;
   //mt.SeedW();
};

void TFastList::Add(int i){
     Items[Count] = i;
     Arr[i] = Count;
     Count++;
}

void TFastList::Delete(int i){
     Count--;
     int pos = Arr[i];
	 assert(pos >=0);
     int item;
     Arr[i] = -1;
     if (pos < Count){
       item = Items[Count];
       Items[pos] = item;
       Arr[item] = pos;
     }
}

void TFastList::Clear(){
     //memset(&Arr,0xFF,sizeof(Arr));//comment if you don't need lookup
     Count = 0;
}

int TFastList::RandItem(threefry4x32_ctr_t& ctr, threefry4x32_key_t& key){
     //return Items[MRandom(Count)];
	assert (Count > 0);
	ctr.v[0]++;
	threefry4x32_ctr_t rand = threefry4x32(ctr, key);
	int it = rand.v[0]%Count;
    return Items[it];
}


void  TFastList::Assign(TFastList* lst){
}

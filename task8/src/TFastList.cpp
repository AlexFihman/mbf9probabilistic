#include "TFastList.h"

TFastList::TFastList() {
    Count = 0;
    for (int i = 0;i < ArrSize;i++) {
        Arr[i] = -1;
        Items[i] = -1;
    }
    //mt = MTRand();
};

void TFastList::Add(int Item) {
    int pos = Arr[Item];
    if (pos == -1) {
        Items[Count] = Item;
        Arr[Item] = Count;
        Count++;
    }
    //assert(Count<ArrSize);
}

void TFastList::Delete(int Item) {
    int pos = Arr[Item];
    if (pos != -1) {
        Arr[Item] = -1;
        Count--;
        if (pos != Count) {
            Item = Items[Count];
            Items[pos] = Item;
            Arr[Item] = pos;
        }
    }
}

void TFastList::Clear() {
    Count = 0;
    for (int i = 0; i < ArrSize; i++)
        Arr[i] = -1;
}

//int TFastList::RandItem() {
//    //return Items[MRandom(Count)];
//    //assert(Count>0);
//    return Items[mt.randInt() % Count];
//}


void  TFastList::Assign(TFastList* lst) {
}


#pragma once

//todo: rename
constexpr int d3 = 9;
constexpr int d1 = 1 << d3;      // {2^d3-1}
constexpr int ArrSize = d1;

class TFastList {
public:
	TFastList();
	int           Count;
	int  Arr[ArrSize];
	int  Items[ArrSize];

	void  Add(int Item);
	void  Delete(int Item);
	void  Clear();
	//int  RandItem();
	void  Assign(TFastList* lst);
private:
	//MTRand mt;
};


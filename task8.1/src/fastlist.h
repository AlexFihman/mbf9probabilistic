#ifndef fastlist
#define fastlist


class TFastList{
public:
	TFastList();
	int           Count;
	int  Arr[65536];
	int  Items[65536];

	void  Add(int i);
	void  Delete(int i);
	void  Clear();
	int  RandItem(threefry4x32_ctr_t& ctr, threefry4x32_key_t& key);
	void  Assign(TFastList* lst);
};
#endif

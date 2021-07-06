#include <Random123/threefry.h>
#include "fastlist.h"
#include "mbf16c.h"
#include "fastlist.h"
//---------------------------------------------------------------------------

#ifndef uMbfObjH
#define uMbfObjH
//---------------------------------------------------------------------------


class TMbfObj {

public:

  TMbfObj*  ParentObj;
  bool arr[d1];
  TFastList* InList[d3];
  TFastList* OutList[d3];
  char   lvl[d3];
  char   LvlGrey[d3];

  TMbfObj();
  ~TMbfObj();  
  void AddItem(int item);
  void AddRandItem(int curr_lvl, threefry4x32_ctr_t& ctr, threefry4x32_key_t& key);
  void AddRandItem2(int curr_lvl, threefry4x32_ctr_t& ctr, threefry4x32_key_t& key);
  void ClearLevel(int curr_lvl);  
};

long double SumOfMbf(TMbfObj* mbfobj, int CurrLvl, long double CurrSum, threefry4x32_ctr_t& ctr, threefry4x32_key_t& key);
//long double C(int x, int y);

#endif

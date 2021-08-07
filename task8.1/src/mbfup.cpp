// mbfup.cpp : Defines the entry point for the console application.
//
#include <cstdlib>
#include <iostream>
#include <thread>
#include <mutex>
#include <iostream>
#include <Random123/threefry.h>
#include "mbf16c.h"
#include "uMbfObj.h"
#include "time_ms.h"


using namespace std;

std::mutex mtx;
static const int num_threads = 4;
static int num_loops = 10000000;
static int c1 = d2 >> 1;
static int c2 = d2 - c1;
static int c3;

int loop_no;

int init_value;

void thread_func(int tid) {
	threefry4x32_ctr_t  ctr = {{0,0,0,0}};
	threefry4x32_key_t key = {{17,15,0,0}};
	TMbfObj mbfObj1;
	TMbfObj mbfObj2;

	long double sum = 0;
	long double sums = 0;
	double t0 = TimeMillis();

	while (true) {
		mtx.lock();
		int func_no = loop_no++;
		mtx.unlock();
		if (func_no > num_loops)
		    return;

		ctr.v[0] = 0;
		ctr.v[1] = func_no;
		ctr.v[2] = init_value;

		sum = 0;
		mbfObj1.ClearLevel(c2);
		mbfObj2.ClearLevel(c1);
		for (int i=0;i<=c3;i++){
			for (int j=0;j<mbfObj1.InList[c2]->Count;j++){
				int item = mbfObj1.InList[c2]->Items[j];
				//std::cout << "item: " << item << std::endl;
				//std::cout << "level of item: " << levelofitem[item] << std::endl;
				item ^= d1-1;
				mbfObj2.AddItem(item);
			}
			long double sum1 = SumOfMbf(&mbfObj1,c2+1,1, ctr, key);
			long double sum2 = SumOfMbf(&mbfObj2,c1+1,1, ctr, key);
			sum += sum1*sum2*C[c3][i];
			if (i<c3)
			{
				mbfObj1.AddRandItem(c2, ctr, key);
				mbfObj2.ClearLevel(c1);
			}						
		}
		sums += sum;
		mtx.lock();
		std::cout << tid << "\t" << func_no << "\t" << sum << "\t" << (TimeMillis()-t0) << std::endl;
		mtx.unlock();
	}	
}

int main(int argc, char* argv[])
{
	//HANDLE cs = GetCurrentThread();
	//SetThreadPriority(cs,-15);	
	init_value = atol(argv[1]);	
	/* we illustrate one user-specified seed and one constant as the key */	

	start();
	c3 = leveldepth[c1];
	std::cout.precision(15);

	std::thread t[num_threads];

	for (int i = 0; i < num_threads; ++i) {
		t[i] = std::thread(thread_func, i);
	}

	for (int i = 0; i < num_threads; ++i) {
		t[i].join();
	}

	return 0;
}
